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

#include <soc/rtc.h>
#include <hal/rtc_io_types.h>
#include <driver/rtc_io.h>
#include <esp_core_dump.h>
#include "TinyWatchy.h"
#include "NTPClient.h"
#include "WiFiHelper.h"
#include "MenuOptions/MenuOption.h"
#include "MenuOptions/NTPOption.h"


BMA423 TinyWatchy::_accelerometer;
bool TinyWatchy::_accelerometerStatus = false;
bool TinyWatchy::_displayFullInit = true;

TinyWatchy::TinyWatchy() : _display(WatchyDisplay(-1, DISPLAY_DC, DISPLAY_RES, DISPLAY_BUSY)),
        _screen(&_display, _screenInfo, &_nvs), _ntp(),
        _alarmHandler(&_accelerometer, &_accelerometerStatus, &_nvs),
        _menu(&_ntp, &_accelerometer, &_screen, &_nvs, &_alarmHandler) {
}

void TinyWatchy::setup() {
    Serial.begin(115200);

    setupHardware();

    uint16_t gpioInterrupts = MCP23018::readRegister(INTCAP);

    if (!_displayFullInit) {
        if (MCP23018::checkBit(gpioInterrupts, ACC_INT_1_PIN)) {
            _accelerometer.getINT();
        }

        MCP23018::clearInterrupt();
    }

    updateData();

    handleWakeUp();

    deepSleep();
}

void TinyWatchy::handleWakeUp() {
    esp_sleep_wakeup_cause_t reason = esp_sleep_get_wakeup_cause();

    switch (reason) {
        case ESP_SLEEP_WAKEUP_TIMER:
            updateMenu();
            _alarmHandler.handle(&_screenInfo);
            _screen.update(true);
            break;
        case ESP_SLEEP_WAKEUP_GPIO:
        case ESP_SLEEP_WAKEUP_EXT1:
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
        _displayFullInit = false;
    }
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);

    _expander.setPinState(DISPLAY_CS, HIGH);
    initRTCIO();

    _alarmHandler.setNextAlarm(_screenInfo.time);

    esp_sleep_enable_ext1_wakeup(((uint64_t)(((uint64_t)1) << MCP_INTERRUPT_PIN)), ESP_EXT1_WAKEUP_ANY_LOW);
    esp_deep_sleep_start();
}

void TinyWatchy::updateBatteryVoltage() {
    float voltage = static_cast<float>(analogReadMilliVolts(BATT_ADC_PIN)) / 710.094f;

    voltage *= 100.f;
    voltage = std::round(voltage);
    voltage /= 100.f;

    voltage = std::clamp(voltage, 3.6f, 4.2f);
    voltage -= 3.6f;
    voltage = std::round(voltage * 166.6666666666667f);

    _screenInfo.battery = static_cast<uint8_t>(voltage);
}

void TinyWatchy::updateData() {
    struct tm time = YatchyTime::getTime();
    _screenInfo.time = time;

    updateBatteryVoltage();

    _screenInfo.charging = !_expander.digitalRead(CHARGING_STATUS_PIN);

    _screenInfo.humanInSleep = (_screenInfo.time.tm_hour >= SLEEP_START && _screenInfo.time.tm_hour < SLEEP_END);
    if (!_displayFullInit && _accelerometerStatus) {
        _screenInfo.steps = _accelerometer.getCounter();
    }

    if (_displayFullInit) {
        int64_t drift = _nvs.getInt("drift", 0);
        bool driftFast = _nvs.getInt("drift_fast", 0);
        if (drift != 0) {

        }
    }
}

void TinyWatchy::updateMenu() {
    _screenInfo.title = _menu.getTitle();
    _screenInfo.description = _menu.getDescription();
    _screenInfo.onMainOption = _menu.isMainOption();
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
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
        .edge_ctrl = BMA4_LEVEL_TRIGGER,
        .lvl       = BMA4_ACTIVE_LOW,
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
#pragma clang diagnostic pop

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

void TinyWatchy::dumpCoreDumpInfo() {
    esp_core_dump_init();
    auto *summary = static_cast<esp_core_dump_summary_t *>(malloc(sizeof(esp_core_dump_summary_t)));
    if (summary) {
        esp_log_level_set("esp_core_dump_elf", ESP_LOG_VERBOSE);
        printf("Retrieving core dump summary..\n");
        esp_err_t err = esp_core_dump_get_summary(summary);
        if (err == ESP_OK) {
            printf("Getting core dump summary ok.\n");
            char panic_reason[200];
            esp_err_t err = esp_core_dump_get_panic_reason(panic_reason, sizeof(panic_reason));
            if (err == ESP_OK) {
                Serial.println(panic_reason);
            }
            esp_core_dump_image_erase();
        } else {
            printf("Getting core dump summary not ok. Error: %d\n", (int) err);
            printf("Probably no coredump present yet.\n");
            printf("esp_core_dump_image_check() = %d\n", esp_core_dump_image_check());
        }
        free(summary);
    }
    fflush(stdout);
}

void TinyWatchy::deinitRTCIO() {
    auto setupRTCIO = [](int pin) {
        auto gpio_pin = gpio_num_t(pin);
        ESP_ERROR_CHECK(rtc_gpio_deinit(gpio_pin));
        ESP_ERROR_CHECK(rtc_gpio_hold_dis(gpio_pin));
    };

    setupRTCIO(DISPLAY_RES);
    setupRTCIO(DISPLAY_DC);
    setupRTCIO(DISPLAY_BUSY);
    setupRTCIO(SPI_MOSI);
    setupRTCIO(SPI_SCK);

    pinMode(DISPLAY_DC, OUTPUT);
    pinMode(DISPLAY_RES, OUTPUT);
    pinMode(DISPLAY_BUSY, INPUT);
}

void TinyWatchy::initRTCIO() {
    auto setupRTCIO = [](int pin, rtc_gpio_mode_t direction) {
        auto gpio_pin = gpio_num_t(pin);
        ESP_ERROR_CHECK(rtc_gpio_init(gpio_pin));
        ESP_ERROR_CHECK(rtc_gpio_set_direction(gpio_pin, direction));
        ESP_ERROR_CHECK(rtc_gpio_pulldown_dis(gpio_pin));
        ESP_ERROR_CHECK(rtc_gpio_pullup_dis(gpio_pin));
    };

    setupRTCIO(DISPLAY_RES, RTC_GPIO_MODE_OUTPUT_ONLY);
    ESP_ERROR_CHECK(rtc_gpio_set_level(gpio_num_t(DISPLAY_RES), true));
    setupRTCIO(DISPLAY_DC, RTC_GPIO_MODE_OUTPUT_ONLY);
    rtc_gpio_set_drive_capability(gpio_num_t(DISPLAY_DC), GPIO_DRIVE_CAP_3);
    ESP_ERROR_CHECK(rtc_gpio_set_level(gpio_num_t(DISPLAY_DC), false));
    setupRTCIO(DISPLAY_BUSY, RTC_GPIO_MODE_INPUT_ONLY);
    setupRTCIO(SPI_MOSI, RTC_GPIO_MODE_OUTPUT_ONLY);
    setupRTCIO(SPI_SCK, RTC_GPIO_MODE_OUTPUT_ONLY);
}

void TinyWatchy::setupHardware() {
    rtc_clk_32k_enable(true);
    rtc_clk_slow_freq_set(RTC_SLOW_FREQ_32K_XTAL);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "ArgumentSelectionDefects"
    Wire.begin(SCL, SDA, 100 * 1000);
#pragma clang diagnostic pop
    Wire.setTimeOut(100);

    _nvs.begin();
    _expander.init();
    _expander.setPinMode(DISPLAY_CS, MCP_OUTPUT);
    _expander.setPinState(DISPLAY_CS, LOW);

    _expander.setPinMode(MCP_5V, MCP_INPUT);
    for (int i = 0; i < 4; i++) {
        _expander.setPinMode(i, MCP_INPUT);
        _expander.setInterruptCause(i, true, false);
        _expander.setPinPullUp(i, true);
        _expander.setInterrupt(i, true);
    }

    _expander.setPinMode(ACC_INT_1_PIN, MCP_INPUT);
    _expander.setInterruptCause(ACC_INT_1_PIN, true, false);
    _expander.setInterrupt(ACC_INT_1_PIN, true);

    _expander.setPinMode(CHARGING_STATUS_PIN, MCP_INPUT);
    _expander.setInterrupt(CHARGING_STATUS_PIN, true);

    _expander.setPinPullUp(10, true);
    _expander.setPinPullUp(11, true);
    _expander.setPinPullUp(12, true);

    deinitRTCIO();

    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SPI_SS);
    _display.epd2.selectSPI(SPI, SPISettings(20000000, MSBFIRST, SPI_MODE0));
    _display.init(0, _displayFullInit, 10, true);
    _display.epd2.setBusyCallback(TinyWatchy::displayBusyCallbackHelper, this);
}
