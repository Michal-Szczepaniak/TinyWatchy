/*

This file is part of TinyWatchy.
Copyright 2023, Michał Szczepaniak <m.szczepaniak.000@gmail.com>

TinyWatchy is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

TinyWatchy is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with TinyWatchy. If not, see <http://www.gnu.org/licenses/>.

*/

#include "TinyWatchy.h"
#include "NTPClient.h"
#include "WiFiHelper.h"
#include "MenuOptions/MenuOption.h"
#include "MenuOptions/NTPOption.h"

SmallRTC TinyWatchy::_smallRTC;
BMA423 TinyWatchy::_accelerometer;
bool TinyWatchy::_accelerometerStatus = false;
bool TinyWatchy::_displayFullInit = true;

TinyWatchy::TinyWatchy() : _display(WatchyDisplay(DISPLAY_CS, DISPLAY_DC, DISPLAY_RES, DISPLAY_BUSY)),
        _screen(&_display, _screenInfo, &_nvs), _ntp(&_smallRTC),
        _alarmHandler(&_smallRTC, &_accelerometer, &_accelerometerStatus, &_nvs),
        _menu(&_ntp, &_accelerometer, &_smallRTC, &_screen, &_nvs, &_alarmHandler) {
}

void TinyWatchy::setup() {
    esp_sleep_wakeup_cause_t wakeupReason = esp_sleep_get_wakeup_cause();

    pinMode(VIB_MOTOR_PIN, OUTPUT);
    digitalWrite(VIB_MOTOR_PIN, LOW);
    gpio_hold_dis((gpio_num_t)VIB_MOTOR_PIN);

    Wire.begin(SDA, SCL);
    _nvs.begin();
    if (_displayFullInit) {
        _smallRTC.init();
    }

    _display.epd2.selectSPI(SPI, SPISettings(20000000, MSBFIRST, SPI_MODE0));
    _display.init(0, _displayFullInit, 10, true);
    _display.epd2.setBusyCallback(TinyWatchy::displayBusyCallbackHelper, this);

    updateData();

    handleWakeUp(wakeupReason);

    deepSleep();
}

void TinyWatchy::handleWakeUp(esp_sleep_wakeup_cause_t reason) {
    switch (reason) {
        case ESP_SLEEP_WAKEUP_EXT0:
            updateMenu();
            _alarmHandler.handle(&_screenInfo);
            _screen.update(true);
            break;
        case ESP_SLEEP_WAKEUP_EXT1:
            if (esp_sleep_get_ext1_wakeup_status() & ACC_INT_MASK) {
                _accelerometer.getINT();
            }
            _menu.handleButtonPress();
            updateMenu();
            _screen.update(true);
            break;
        default:
            setupAccelerometer();
            updateMenu();
            _screen.update(false);
            break;
    }
}

void TinyWatchy::deepSleep() {
    _display.hibernate();

    if (_displayFullInit) {
        esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
        _displayFullInit = false;
    }

    _alarmHandler.setNextAlarm(_screenInfo.time);

    const uint64_t ignore = 0b11110001000000110000100111000010;
    for (int i = 0; i < GPIO_NUM_MAX; i++) {
        if ((ignore >> i) & 0b1)
            continue;
        pinMode(i, INPUT);
    }

    esp_sleep_enable_ext0_wakeup((gpio_num_t) RTC_INT_PIN, 0);
    esp_sleep_enable_ext1_wakeup(RIGHT_BTN_MASK | LEFT_BTN_MASK | BACK_BTN_MASK |
                                 SELECT_BTN_MASK | ACC_INT_MASK,
                                 ESP_EXT1_WAKEUP_ANY_HIGH);
    esp_deep_sleep_start();
}

void TinyWatchy::updateBatteryVoltage() {
    float voltage = static_cast<float>(analogReadMilliVolts(BATT_ADC_PIN)) / 1000.0f * 2.0f;
    voltage *= 100.f;
    voltage = std::floor(voltage);
    voltage /= 100.f;

    voltage = std::clamp(voltage, 3.6f, 4.2f);
    voltage -= 3.6f;
    voltage = std::round(voltage * 166.6666666666667f);

    _screenInfo.battery = static_cast<uint8_t>(voltage);
}

void TinyWatchy::updateData() {
    _smallRTC.read((tmElements_t &) _screenInfo.time);
    updateBatteryVoltage();

    _screenInfo.humanInSleep = (_screenInfo.time.hour >= SLEEP_START && _screenInfo.time.hour < SLEEP_END);
    if (!_displayFullInit && _accelerometerStatus) {
        _screenInfo.steps = _accelerometer.getCounter();
    }

    if (_displayFullInit) {
        int64_t drift = _nvs.getInt("drift", 0);
        bool driftFast = _nvs.getInt("drift_fast", 0);
        if (drift != 0) {
            _smallRTC.setDrift(drift, driftFast, false);
        }
    }
}

void TinyWatchy::updateMenu() {
    _screenInfo.title = _menu.getTitle();
    _screenInfo.description = _menu.getDescription();
    _screenInfo.onMainOption = _menu.isMainOption();
}

void TinyWatchy::setupAccelerometer() {
    if (!_accelerometer.begin(&TinyWatchy::readRegisterHelper, &TinyWatchy::writeRegisterHelper, delay)) {
        return;
    }

    Acfg cfg = {
        .odr = BMA4_OUTPUT_DATA_RATE_100HZ,
        .bandwidth = BMA4_ACCEL_NORMAL_AVG4,
        .perf_mode = BMA4_CIC_AVG_MODE,
        .range = BMA4_ACCEL_RANGE_2G,
    };

    _accelerometer.setAccelConfig(cfg);
    _accelerometer.wakeUp();
    _accelerometer.enableAccel();
    _accelerometerStatus = true;

    struct bma4_int_pin_config config = {
        .edge_ctrl = BMA4_EDGE_TRIGGER,
        .lvl       = BMA4_ACTIVE_HIGH,
        .od        = BMA4_PUSH_PULL,
        .output_en = BMA4_OUTPUT_ENABLE,
        .input_en  = BMA4_INPUT_DISABLE,
    };

    _accelerometer.setINTPinConfig(config, BMA4_INTR1_MAP);

    struct bma423_axes_remap remap_data = {
            .x_axis      = 1,
            .x_axis_sign = 0,
            .y_axis      = 0,
            .y_axis_sign = 0,
            .z_axis      = 2,
            .z_axis_sign = 1,
    };

    _accelerometer.setRemapAxes(&remap_data);
    _accelerometer.enableFeature(BMA423_STEP_CNTR, true);
    _accelerometer.enableFeature(BMA423_WAKEUP, true);

    _accelerometer.resetStepCounter();

    _accelerometer.enableWakeupInterrupt();
}

uint16_t TinyWatchy::readRegisterHelper(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len) {
    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom((uint8_t) address, (uint8_t) len);
    uint8_t i = 0;
    while (Wire.available()) {
        data[i++] = Wire.read();
    }
    return 0;
}

uint16_t TinyWatchy::writeRegisterHelper(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len) {
    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.write(data, len);
    return (0 != Wire.endTransmission());
}
