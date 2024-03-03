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

#include "UwUFace.h"
#include "resources.h"
#include "Watchy/icons.h"

const unsigned char* const UwUFace::EMOTES[] = {
    resources::ANGRY_IMAGE,
    resources::CONCERN_IMAGE,
    resources::COPIUM_IMAGE,
    resources::JAM_IMAGE,
    resources::NOTE_IMAGE,
    resources::OWO_IMAGE,
    resources::POG_IMAGE,
    resources::SHY_IMAGE,
    resources::SIP_IMAGE,
    resources::SMUG_IMAGE,
};
long UwUFace::_lastEmote = 0;

UwUFace::UwUFace(GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> *display) : AbstractFace(display) {

}

void UwUFace::draw(const ScreenInfo &screenInfo) {
    _display->drawBitmap(0, 25, resources::POINT_IMAGE, POINT_IMAGE_WIDTH, POINT_IMAGE_HEIGHT, GxEPD_BLACK);

    long rnd = 0;
    do {
        rnd = random(0, 9);
    } while (rnd == _lastEmote);
    _lastEmote = rnd;

    _display->drawBitmap(
        200-JAM_IMAGE_WIDTH-5,
        200-JAM_IMAGE_HEIGHT-15,
        EMOTES[rnd],
        50,
        50,
        GxEPD_BLACK
    );

    drawTime(screenInfo.time);
    drawBattery(screenInfo.battery);
    drawSteps(screenInfo.steps);
    drawMenuTitle(screenInfo.title);
    drawMenuDescription(screenInfo.description);
}

void UwUFace::drawTime(const DateTime &time) {
    _display->setFont(&resources::EIGHT_BIT_OPERATOR_PLUS_BOLD_22);
    _display->setTextColor(GxEPD_BLACK);
    _display->setCursor(70, 40);

    if (time.hour < 10) {
        _display->print("0");
    }

    _display->print(time.hour);
    _display->setCursor(124, 38);
    _display->print(":");

    _display->setCursor(140, 40);
    if (time.minute < 10) {
        _display->print("0");
    }

    _display->println(time.minute);
}

void UwUFace::drawBattery(const uint8_t &battery) {
    drawBatteryIcon(90, 55, battery, _display);
    _display->setFont(&resources::EIGHT_BIT_OPERATOR_PLUS_REGULAR_12);
    _display->setCursor(130, 73);
    _display->print(String(battery) + "%");
}

void UwUFace::drawSteps(const unsigned int &steps) {
    _display->setFont(&resources::EIGHT_BIT_OPERATOR_PLUS_REGULAR_12);
    _display->drawBitmap(100, 84, ICON_STEPS, 19, 23, GxEPD_BLACK);
    _display->setCursor(130, 103);
    _display->println(steps);
}

void UwUFace::drawMenuTitle(const std::string &title) {
    int16_t x, y;
    uint16_t w, h;
    String text(title.c_str());
    _display->setFont(&resources::EIGHT_BIT_OPERATOR_PLUS_BOLD_10);
    _display->getTextBounds(text, 0, 0, &x, &y, &w, &h);

    _display->setCursor(static_cast<int16_t>(70-(w/2)), 150);
    _display->print(title.c_str());
}

void UwUFace::drawMenuDescription(const std::string &description) {
    int16_t x, y;
    uint16_t w, h;
    String text(description.c_str());
    _display->setFont(&resources::EIGHT_BIT_OPERATOR_PLUS_BOLD_6);
    _display->getTextBounds(text, 0, 0, &x, &y, &w, &h);

    _display->setCursor(static_cast<int16_t>(70-(w/2)), 180);
    _display->print(description.c_str());
}
