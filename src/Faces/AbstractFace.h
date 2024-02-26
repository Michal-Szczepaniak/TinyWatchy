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
#include "ScreenInfo.h"
#include <GxEPD2_BW.h>

class AbstractFace {
public:
    explicit AbstractFace(GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> *display) : _display(display) {};
    virtual ~AbstractFace() = default;

    virtual void draw(ScreenInfo *screenInfo) = 0;

    virtual std::string getName() = 0;

protected:
    GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> *_display;
};

#endif //TINYWATCHY_ABSTRACTFACE_H
