#include "DefaultFace.h"
#include "Watchy/DSEG7_Classic_Bold_53.h"
#include "Watchy/DSEG7_Classic_Bold_25.h"
#include "Watchy/Seven_Segment10pt7b.h"
#include "Watchy/icons.h"

DefaultFace::DefaultFace(GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> *display) : AbstractFace(display) {

}

void DefaultFace::draw(const ScreenInfo &screenInfo) {
    _display->fillRect(0, 0, 200, 2, screenInfo.suspend ? GxEPD_WHITE : GxEPD_BLACK);
    drawTime(screenInfo.time);
    drawBattery(screenInfo.battery);
    drawSteps(screenInfo.steps);
    drawMenuTitle(screenInfo.title);
    drawMenuDescription(screenInfo.description);
}

void DefaultFace::drawTime(const DateTime &time) {
    _display->setFont(&DSEG7_Classic_Bold_53);
    _display->setTextColor(GxEPD_BLACK);
    _display->setCursor(5, 53 + 10);

    if (time.hour < 10) {
        _display->print("0");
    }

    _display->print(time.hour);
    _display->print(":");

    if (time.minute < 10) {
        _display->print("0");
    }

    _display->println(time.minute);
}

void DefaultFace::drawBattery(const uint8_t &battery) {
    _display->drawBitmap(150, 75, ICON_BATTERY, 37, 21, GxEPD_BLACK);
    _display->fillRect(155, 80, 27, 11, GxEPD_WHITE);
    _display->fillRect(155, 80, static_cast<int16_t>(std::round(static_cast<float>(battery) / 4)), 11, GxEPD_BLACK);
    _display->setFont(&Seven_Segment10pt7b);
    _display->setCursor(108, 91);
    _display->print(String(battery) + "%");
}

void DefaultFace::drawSteps(const unsigned int &steps) {
    _display->setFont(&DSEG7_Classic_Bold_25);
    _display->drawBitmap(10, 73, ICON_STEPS, 19, 23, GxEPD_BLACK);
    _display->setCursor(35, 98);
    _display->println(steps);
}

void DefaultFace::drawMenuTitle(const std::string &title) {
    int16_t x, y;
    uint16_t w, h;
    String text(title.c_str());
    _display->setFont(&Seven_Segment10pt7b);
    _display->getTextBounds(text, 0, 0, &x, &y, &w, &h);

    _display->setCursor(static_cast<int16_t>(100-(w/2)), 140);
    _display->print(title.c_str());
}

void DefaultFace::drawMenuDescription(const std::string &description) {
    int16_t x, y;
    uint16_t w, h;
    String text(description.c_str());
    _display->setFont(&Seven_Segment10pt7b);
    _display->getTextBounds(text, 0, 0, &x, &y, &w, &h);

    _display->setCursor(static_cast<int16_t>(100-(w/2)), 180);
    _display->print(description.c_str());
}
