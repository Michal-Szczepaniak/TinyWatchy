#include <WiFiUdp.h>
#include <WiFi.h>
#include "TinyWatchy.h"
#include "NTPClient.h"

bool TinyWatchy::_displayFullInit = true;
BMA423 TinyWatchy::_sensor;

TinyWatchy::TinyWatchy() : _display(new GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT>(
        WatchyDisplay(DISPLAY_CS, DISPLAY_DC, DISPLAY_RES, DISPLAY_BUSY))),
                           _screen(_display.get(), _screenInfo) {
}

void TinyWatchy::setup() {
    esp_sleep_wakeup_cause_t wakeupReason;
    wakeupReason = esp_sleep_get_wakeup_cause();

    Serial.begin(115200);
    Serial.println("Hello");

    _smallRTC.init();

    _display->epd2.selectSPI(SPI, SPISettings(20000000, MSBFIRST, SPI_MODE0));
    _display->init(0, _displayFullInit, 10,
                   true);
    _display->epd2.setBusyCallback(TinyWatchy::displayBusyCallbackHelper, this);

    updateData();
    _screenInfo.suspend = false;

    switch (wakeupReason) {
        case ESP_SLEEP_WAKEUP_EXT0:
            _sensor.getINT();
            _screen.update(true);
            break;
        case ESP_SLEEP_WAKEUP_EXT1:
            _screen.update(true);
            break;
        default:
            setupAccelerometer();
            syncNTP();
            _screen.update(false);
            break;
    }

    _screenInfo.suspend = true;
    _screen.update(true);

    deepSleep();
}

void TinyWatchy::deepSleep() {
    _display->hibernate();
    if (_displayFullInit)
        esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
    _displayFullInit = false;
    _smallRTC.clearAlarm();
    _smallRTC.nextMinuteWake(true);

    const uint64_t ignore = 0b11110001000000110000100111000010;
    for (int i = 0; i < GPIO_NUM_MAX; i++) {
        if ((ignore >> i) & 0b1)
            continue;
        pinMode(i, INPUT);
    }

    esp_sleep_enable_ext0_wakeup((gpio_num_t)RTC_INT_PIN, 0);
    esp_sleep_enable_ext1_wakeup(MENU_BTN_MASK|BACK_BTN_MASK|UP_BTN_MASK|DOWN_BTN_MASK|ACC_INT_1_PIN, ESP_EXT1_WAKEUP_ANY_HIGH);
    esp_deep_sleep_start();
}

void TinyWatchy::updateBatteryVoltage() {
    float voltage = static_cast<float>(analogReadMilliVolts(BATT_ADC_PIN)) / 1000.0f * 2.0f;

    voltage = std::clamp(voltage, 3.8f, 4.1f);
    voltage -= 3.8;
    voltage = std::round(voltage * 333.333f);

    _screenInfo.battery = static_cast<uint8_t>(voltage);
//    _screenInfo.title = "Menu";
//    _screenInfo.description = "Option Option Option";
}

void TinyWatchy::updateData() {
    _smallRTC.read((tmElements_t&)_screenInfo.time);
    updateBatteryVoltage();
    if (!_displayFullInit)
        _screenInfo.steps = _sensor.getCounter();
}

void TinyWatchy::setupAccelerometer() {
    if (!_sensor.begin(&TinyWatchy::readRegisterHelper, &TinyWatchy::writeRegisterHelper, delay)) {
        return;
    }

    Acfg cfg = {
            .odr = BMA4_OUTPUT_DATA_RATE_200HZ,
            .bandwidth = BMA4_ACCEL_NORMAL_AVG4,
            .perf_mode = BMA4_CIC_AVG_MODE,
            .range = BMA4_ACCEL_RANGE_2G,
    };

    _sensor.setAccelConfig(cfg);
    _sensor.enableAccel();
    _sensor.shutDown();

    struct bma4_int_pin_config config = {
            .edge_ctrl = BMA4_EDGE_TRIGGER,
            .lvl       = BMA4_ACTIVE_LOW,
            .od        = BMA4_PUSH_PULL,
            .output_en = BMA4_OUTPUT_ENABLE,
            .input_en  = BMA4_INPUT_DISABLE,
    };

    _sensor.setINTPinConfig(config, BMA4_INTR1_MAP);

    struct bma423_axes_remap remap_data = {
            .x_axis      = 1,
            .x_axis_sign = 0,
            .y_axis      = 0,
            .y_axis_sign = 0,
            .z_axis      = 2,
            .z_axis_sign = 1,
    };

    _sensor.setRemapAxes(&remap_data);
    _sensor.enableFeature(BMA423_STEP_CNTR, true);
    _sensor.enableFeature(BMA423_WAKEUP, true);

    _sensor.resetStepCounter();

    _sensor.enableWakeupInterrupt();
}

uint16_t TinyWatchy::readRegisterHelper(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len) {
    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom((uint8_t)address, (uint8_t)len);
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

void TinyWatchy::syncNTP() {
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.setHostname("TinyWatchy");
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    WiFi.waitForConnectResult();
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.disconnect();
        WiFi.mode(WIFI_OFF);

        return;
    }

    WiFiUDP ntpUDP;
    NTPClient timeClient(ntpUDP);

    timeClient.begin();

    timeClient.update();
    timeClient.setTimeOffset(3600);

    time_t epochTime = timeClient.getEpochTime();

    tmElements_t time;
    _smallRTC.BreakTime(epochTime, time);
    _smallRTC.set(time);
    timeClient.end();

    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
}
