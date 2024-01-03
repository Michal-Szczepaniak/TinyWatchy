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
#include "MenuOptions/MenuOption.h"
#include "MenuOptions/AboutOption.h"

uint8_t Menu::_level = 0;
uint8_t Menu::_optionId = 0;

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

Menu::Menu() {
    _options.emplace_back(new MenuOption);
    _options.emplace_back(new AboutOption);
}

void Menu::handleButtonPress() {
    uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();

    if (!(wakeupBit & (RIGHT_BTN_MASK | LEFT_BTN_MASK | SELECT_BTN_MASK | BACK_BTN_MASK))) {
        return;
    }

    int buttonPressed = _buttonMap.at(BUTTON_MAP).at(getButtonPressed(wakeupBit));
    Serial.printf("Button pressed: %d, mapped %d\n", getButtonPressed(wakeupBit), buttonPressed);

    switch (buttonPressed) {
        case Button::RIGHT:
            Serial.println("right");
            if (!_level)
                nextOption();
            break;
        case Button::LEFT:
            Serial.println("left");
            if (!_level)
                prevOption();
            break;
        case Button::SELECT:
            Serial.println("select");
//            selectOption();
            break;
        case Button::BACK:
            Serial.println("back");
//            if (_level)
//                _level--;
            break;
        default:
            break;
    }
}

std::string Menu::getTitle() {
    Serial.printf("OptionId: %d\n", _optionId);
    return _options[_optionId]->getTitle();
}

std::string Menu::getDescription() {
    return _options[_optionId]->getDescription();
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
    Serial.println("next option");
    if (!_level) {
        _optionId++;

        if (_optionId == _options.size()) {
            _optionId = 0;
        }
    }
}

void Menu::prevOption() {
    Serial.printf("prev option. level: %d, option: %d\n", _level, _optionId);
    if (!_level) {
        if (_optionId == 0) {
            _optionId = _options.size()-1;
            Serial.printf("changed option: %d\n", _optionId);
        } else {
            _optionId--;
            Serial.printf("decreased option: %d\n", _optionId);
        }
    }
}

void Menu::selectOption() {
    Serial.println("select option");
    if (!_level) {
        _level++;
    } else {
        if (_options[_optionId]->onSelectButtonPressed()) {
            _level++;
        }
    }
}
