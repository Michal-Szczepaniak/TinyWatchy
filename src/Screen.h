#ifndef TINYWATCHY_SCREEN_H
#define TINYWATCHY_SCREEN_H

#include "Watchy/Display.h"
#include "Faces/AbstractFace.h"
#include <GxEPD2_BW.h>
#include <memory>

class Screen {
public:
    explicit Screen(GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> *display, const ScreenInfo &screenInfo);
    void update(bool partial = false);

private:
    GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> *_display;
    std::unique_ptr<AbstractFace> _face;
    const ScreenInfo &_screenInfo;
};


#endif //TINYWATCHY_SCREEN_H
