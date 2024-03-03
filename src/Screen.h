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

#ifndef TINYWATCHY_SCREEN_H
#define TINYWATCHY_SCREEN_H

#include "Watchy/Display.h"
#include "Faces/AbstractFace.h"
#include "ArduinoNvs.h"
#include <GxEPD2_BW.h>
#include <memory>

class Screen {
public:
    explicit Screen(GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> *display, const ScreenInfo &screenInfo, ArduinoNvs *nvs);
    void update(bool partial = false);
    const std::vector<std::unique_ptr<AbstractFace>>& getFaces() const;

private:
    GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> *_display;
    std::vector<std::unique_ptr<AbstractFace>> _faces;
    const ScreenInfo &_screenInfo;
    ArduinoNvs *_nvs;
};


#endif //TINYWATCHY_SCREEN_H
