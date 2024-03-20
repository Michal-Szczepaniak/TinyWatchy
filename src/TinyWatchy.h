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
#include "NTP.h"
#include "Menu.h"
#include "AlarmHandler.h"

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
    void handleWakeUp(esp_sleep_wakeup_cause_t reason);
    void deepSleep();
    void updateData();
    void updateBatteryVoltage();
    void updateMenu();
    void setupAccelerometer();

    static uint16_t readRegisterHelper(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);
    static uint16_t writeRegisterHelper(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);

private:
    ArduinoNvs _nvs;
    GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> _display;
    ScreenInfo _screenInfo;
    Screen _screen;
    NTP _ntp;
    Menu _menu;
    AlarmHandler _alarmHandler;
    RTC_DATA_ATTR static SmallRTC _smallRTC;
    RTC_DATA_ATTR static BMA423 _accelerometer;
    RTC_DATA_ATTR static bool _accelerometerStatus;
    RTC_DATA_ATTR static bool _displayFullInit;
};

#endif //TINYWATCHY_TINYWATCHY_H
