#include "Screen.h"
#include "Faces/DefaultFace.h"

Screen::Screen(GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> *display, const ScreenInfo &screenInfo) :
        _display(display), _face(new DefaultFace(display)), _screenInfo(screenInfo) {

}

void Screen::update(bool partial) {
    _display->setFullWindow();
    _display->fillScreen(GxEPD_WHITE);

    _face->draw(_screenInfo);

    _display->display(partial);
}
