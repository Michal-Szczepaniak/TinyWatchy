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

#include <esp_sleep.h>
#include <Arduino.h>
#include "Menu.h"
#include "buttons.h"
#include "defines.h"
#include "defines_private.h"
#if PRIVATE == 1
#include "MenuOptions/Private/Include.h"
#endif

uint8_t Menu::_currentStackPage = 0;

StackPage Menu::_pageStack[3] = {{}, {}, {}};

const std::map<uint8_t, std::map<uint8_t, int>> Menu::_buttonMap = {
        {
            0,
            {
                {RIGHT_BTN_PIN, Button::RIGHT },
                {LEFT_BTN_PIN, Button::LEFT },
                {SELECT_BTN_PIN, Button::SELECT },
                {BACK_BTN_PIN, Button::BACK },
            }
        },
        {
            1,
            {
                {RIGHT_BTN_PIN, Button::SELECT },
                {LEFT_BTN_PIN, Button::BACK },
                {SELECT_BTN_PIN, Button::RIGHT },
                {BACK_BTN_PIN, Button::LEFT },
            }
        }
};

Menu::Menu(NTP *ntp, BMA423* accelerometer, SmallRTC *smallRTC, Screen *screen, ArduinoNvs *nvs,
           AlarmHandler *alarmHandler) : _ntpOption(ntp),
   _settingsSubmenu("Settings", "Open settings", [this]{ changePage(1); return false; }),
   _alarmSubmenu(">Alarm", "Manage alarm", [this]{ changePage(2); return false; }),
   _accelerometerOption(accelerometer), _watchfaceOption(screen, nvs), _driftOption(ntp, smallRTC, nvs),
   _alarmSetOption(alarmHandler, nvs), _alarmClearOption(nvs)
#if PRIVATE == 1
   , _abstractOption1(PrivateOptions::getOption1())
#endif
   {


}

void Menu::handleButtonPress() {
    uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();

    if (!(wakeupBit & (RIGHT_BTN_MASK | LEFT_BTN_MASK | SELECT_BTN_MASK | BACK_BTN_MASK))) {
        return;
    }

    int buttonPressed = _buttonMap.at(BUTTON_MAP).at(getButtonPressed(wakeupBit));

    switch (buttonPressed) {
        case Button::RIGHT:
            nextOption();
            break;
        case Button::LEFT:
            prevOption();
            break;
        case Button::SELECT:
            selectOption();
            break;
        case Button::BACK:
            backOption();
            break;
        default:
            break;
    }
}

std::string Menu::getTitle() {
    return getCurrentItem()->getTitle();
}

std::string Menu::getDescription() {
    return getCurrentItem()->getDescription(getCurrentStackPage());
}

bool Menu::isMainOption() {
    return _currentStackPage == 0 && getCurrentStackPage().itemIndex == 0;
}

uint8_t Menu::getButtonPressed(const uint64_t &wakeupBit) {
    if (wakeupBit & RIGHT_BTN_MASK) {
        return RIGHT_BTN_PIN;
    } else if (wakeupBit & LEFT_BTN_MASK) {
        return LEFT_BTN_PIN;
    } else if (wakeupBit & SELECT_BTN_MASK) {
        return SELECT_BTN_PIN;
    } else if (wakeupBit & BACK_BTN_MASK) {
        return BACK_BTN_PIN;
    }

    return 0;
}

void Menu::nextOption() {
    StackPage& page = getCurrentStackPage();
    if (page.selected) {
        getCurrentItem()->onNextButtonPressed();
        return;
    }

    const MenuPage& currentPage = getCurrentPage();
    if (page.itemIndex+1 >= currentPage.items.size()) {
        page.itemIndex = 0;
    } else {
        page.itemIndex++;
    }
}

void Menu::prevOption() {
    StackPage& page = getCurrentStackPage();
    if (page.selected) {
        getCurrentItem()->onPrevButtonPressed();
        return;
    }

    const MenuPage& currentPage = getCurrentPage();
    if (page.itemIndex == 0) {
        page.itemIndex = currentPage.items.size()-1;
    } else {
        page.itemIndex--;
    }
}

void Menu::selectOption() {
    bool ret = getCurrentItem()->onSelectButtonPressed(getCurrentStackPage());
    if (ret) {
        getCurrentStackPage().selected = true;
    }
}

void Menu::backOption() {
    getCurrentItem()->onBackButtonPressed();
    if (getCurrentStackPage().selected) {
        getCurrentStackPage().selected = false;
        return;
    }

    if (_currentStackPage > 0) {
        _currentStackPage--;
    }  else {
        getCurrentStackPage().itemIndex = 0;
    }
}

const MenuPage& Menu::getCurrentPage() {
    return _pages[getCurrentStackPage().pageIndex];
}

AbstractOption* Menu::getCurrentItem() {
    return getCurrentPage().items[getCurrentStackPage().itemIndex];
}

StackPage& Menu::getCurrentStackPage() {
    return _pageStack[_currentStackPage];
}

void Menu::changePage(uint8_t menuPage) {
    if (_currentStackPage == 2) return;

    _currentStackPage++;
    getCurrentStackPage().pageIndex = menuPage;
    getCurrentStackPage().itemIndex = 0;
    getCurrentStackPage().selected = false;
}
