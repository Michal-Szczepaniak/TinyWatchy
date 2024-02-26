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

#ifndef TINYWATCHY_ABSTRACTFACE_H
#define TINYWATCHY_ABSTRACTFACE_H

#include "Watchy/Display.h"
#include "Watchy/icons.h"
#include "ScreenInfo.h"
#include <GxEPD2_BW.h>

class AbstractFace {
public:
    explicit AbstractFace(GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> *display) : _display(display) {};
    virtual ~AbstractFace() = default;

    virtual void draw(ScreenInfo *screenInfo) = 0;

    virtual std::string getName() = 0;

    static void drawBatteryIcon(
        int x,
        int y,
        const uint8_t &battery,
        GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> *display
    ) {
        display->drawBitmap(x, y, ICON_BATTERY, 37, 21, GxEPD_BLACK);
        display->fillRect(x+5, y+5, 27, 11, GxEPD_WHITE);
        display->fillRect(x+5, y+5, static_cast<int16_t>(std::round(static_cast<float>(battery) / 4)), 11, GxEPD_BLACK);
    }

protected:
    GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> *_display;
};

#endif //TINYWATCHY_ABSTRACTFACE_H
