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

#ifndef TINYWATCHY_MENU_H
#define TINYWATCHY_MENU_H

#include <string>
#include <map>
#include <vector>
#include <stack>
#include "MenuOptions/AbstractOption.h"
#include "MenuOptions/MenuOption.h"
#include "ScreenInfo.h"
#include "Screen.h"
#include "MenuPage.h"
#include "MenuOptions/NTPOption.h"
#include "MenuOptions/SubMenuOption.h"
#include "MenuOptions/AccelerometerOption.h"
#include "MenuOptions/AboutOption.h"
#include "MenuOptions/VoltageOption.h"
#include "MenuOptions/WatchfaceOption.h"
#include "MenuOptions/DriftOption.h"
#include "MenuOptions/UiOption.h"
#include "AlarmHandler.h"
#include "MenuOptions/AlarmSetOption.h"
#include "MenuOptions/AlarmClearOption.h"

class Menu {
public:
    Menu(NTP *ntp, BMA423* accelerometer, SmallRTC *smallRTC, Screen *screen, ArduinoNvs *nvs,
         AlarmHandler *alarmHandler);
    void handleButtonPress();

    std::string getTitle();
    std::string getDescription();
    bool isMainOption();

private:
    static uint8_t getButtonPressed(const uint64_t &wakeupBit);
    void nextOption();
    void prevOption();
    void selectOption();
    void backOption();
    const MenuPage& getCurrentPage();
    AbstractOption* getCurrentItem();
    StackPage& getCurrentStackPage();
    void changePage(uint8_t menuPage);

private:
    RTC_DATA_ATTR static uint8_t _currentStackPage;
    RTC_DATA_ATTR static StackPage _pageStack[3];
    static const std::map<uint8_t, std::map<uint8_t, int>> _buttonMap;
    MenuOption _menuOption;
    NTPOption _ntpOption;
    SubMenuOption _settingsSubmenu;
    SubMenuOption _alarmSubmenu;
    AboutOption _aboutOption;
    VoltageOption _voltageOption;
    AccelerometerOption _accelerometerOption;
    WatchfaceOption _watchfaceOption;
    DriftOption _driftOption;
    UiOption _uiOption;
    AlarmSetOption _alarmSetOption;
    AlarmClearOption _alarmClearOption;
#if PRIVATE == 1
    AbstractOption *_abstractOption1;
#endif
    const MenuPage _pages[3] = {
        {
            .items = {
                &_menuOption,
                &_ntpOption,
#if PRIVATE == 1
                _abstractOption1,
#endif
                &_settingsSubmenu,
            },
        },
        {
            .items = {
                &_aboutOption,
                &_alarmSubmenu,
                &_accelerometerOption,
                &_voltageOption,
                &_watchfaceOption,
                &_driftOption,
                &_uiOption,
            }
        },
        {
            .items = {
                &_alarmSetOption,
                &_alarmClearOption,
            }
        }
    };
};


#endif //TINYWATCHY_MENU_H
