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

#include "Screen.h"
#include "Faces/DefaultFace.h"

Screen::Screen(GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> *display, ScreenInfo *screenInfo) :
        _display(display), _face(new DefaultFace(display)), _screenInfo(screenInfo) {

}

void Screen::update(bool partial) {
    _display->setFullWindow();
    _display->fillScreen(GxEPD_WHITE);

    _face->draw(_screenInfo);

    _display->display(partial);
}
