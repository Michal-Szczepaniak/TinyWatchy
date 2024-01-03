#ifndef TINYWATCHY_TINYWATCHY_H
#define TINYWATCHY_TINYWATCHY_H

#include <Arduino.h>
#include <SmallRTC.h>
#include "Watchy/bma.h"
#include "Watchy/Display.h"
#include <GxEPD2_BW.h>
#include <memory>
#include "defines.h"
#include "Screen.h"

class TinyWatchy {
public:
    TinyWatchy();

    void setup();

    static void displayBusyCallbackHelper(const void *ctx) {
        gpio_wakeup_enable((gpio_num_t)DISPLAY_BUSY, GPIO_INTR_LOW_LEVEL);
        esp_sleep_enable_gpio_wakeup();
        esp_light_sleep_start();
    }
private:
    void deepSleep();
    void updateData();
    void updateBatteryVoltage();
    void setupAccelerometer();
    void syncNTP();

    static uint16_t readRegisterHelper(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);
    static uint16_t writeRegisterHelper(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);

private:
    SmallRTC _smallRTC;
    std::unique_ptr<GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT>> _display;
    Screen _screen;
    ScreenInfo _screenInfo;
    RTC_DATA_ATTR static BMA423 _sensor;
    RTC_DATA_ATTR static bool _displayFullInit;
};

#endif //TINYWATCHY_TINYWATCHY_H
