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

#ifndef TINYWATCHY_UWUFACE_H
#define TINYWATCHY_UWUFACE_H

#include "AbstractFace.h"
#include <vector>

class UwUFace : public AbstractFace {
public:
    explicit UwUFace(GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> *display);

    void draw(ScreenInfo *screenInfo) override;

private:
    void drawTime(const DateTime &time);
    void drawBattery(const uint8_t &battery);
    void drawSteps(const unsigned int &steps);
    void drawMenuTitle(const std::string &title);
    void drawMenuDescription(const std::string &description);

    static const unsigned char* const EMOTES[];
    RTC_DATA_ATTR static long _lastEmote;
};


#endif //TINYWATCHY_UWUFACE_H
