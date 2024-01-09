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
#include "MenuOptions/AbstractOption.h"
#include "ScreenInfo.h"

class Menu {
public:
    void appendOption(AbstractOption *option);
    void handleButtonPress();

    std::string getTitle();
    std::string getDescription();

private:
    static uint8_t getButtonPressed(const uint64_t &wakeupBit);
    void nextOption();
    void prevOption();
    void selectOption();
    void backOption();

private:
    RTC_DATA_ATTR static uint8_t _level;
    RTC_DATA_ATTR static uint8_t _optionId;
    std::vector<AbstractOption*> _options;
    static const std::map<uint8_t, std::map<uint8_t, int>> _buttonMap;
};


#endif //TINYWATCHY_MENU_H
