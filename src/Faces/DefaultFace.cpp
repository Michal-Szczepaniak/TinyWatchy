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

#include "DefaultFace.h"
#include "resources.h"
#include "Watchy/icons.h"

DefaultFace::DefaultFace(GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> *display) : AbstractFace(display) {

}

void DefaultFace::draw(ScreenInfo *screenInfo) {
    _display->drawBitmap(0, 0, resources::WATCHFACE_DEFAULT_IMAGE, 200, 200, GxEPD_BLACK);
    drawTime(screenInfo->time);
    drawBattery(screenInfo->battery);
    drawSteps(screenInfo->steps);
    drawMenuTitle(screenInfo->title);
    drawMenuDescription(screenInfo->description);
}

void DefaultFace::drawTime(const DateTime &time) {
    _display->setFont(&resources::DIGITAL_DISPLAY_REGULAR_50);
    _display->setTextColor(GxEPD_BLACK);
    _display->setCursor(5, 50 + 18);

    if (time.hour < 10) {
        _display->print("0");
    }

    _display->print(time.hour);
    _display->setCursor(82, 50 + 18);
    _display->print(":");

    _display->setCursor(114, 50 + 18);
    if (time.minute < 10) {
        _display->print("0");
    }

    _display->println(time.minute);
}

void DefaultFace::drawBattery(const uint8_t &battery) {
    drawBatteryIcon(150, 75, battery, _display);
    _display->setFont(&resources::DIGITAL_DISPLAY_REGULAR_15);
    _display->setCursor(108, 94);
    _display->print(String(battery));
}

void DefaultFace::drawSteps(const unsigned int &steps) {
    _display->setFont(&resources::DIGITAL_DISPLAY_REGULAR_15);
    _display->drawBitmap(10, 73, ICON_STEPS, 19, 23, GxEPD_BLACK);
    _display->setCursor(35, 94);
    _display->println(steps);
}

void DefaultFace::drawMenuTitle(const std::string &title) {
    int16_t x, y;
    uint16_t w, h;
    String text(title.c_str());
    _display->setFont(&resources::DIGITAL_7_18);
    _display->getTextBounds(text, 0, 0, &x, &y, &w, &h);

    _display->setCursor(static_cast<int16_t>(100-(w/2)), 140);
    _display->print(title.c_str());
}

void DefaultFace::drawMenuDescription(const std::string &description) {
    int16_t x, y;
    uint16_t w, h;
    String text(description.c_str());
    _display->setFont(&resources::DIGITAL_7_12);
    _display->getTextBounds(text, 0, 0, &x, &y, &w, &h);

    _display->setCursor(static_cast<int16_t>(100-(w/2)), 180);
    _display->print(description.c_str());
}
