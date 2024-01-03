#ifndef TINYWATCHY_ABSTRACTFACE_H
#define TINYWATCHY_ABSTRACTFACE_H

#include "Watchy/Display.h"
#include "ScreenInfo.h"
#include <GxEPD2_BW.h>

class AbstractFace {
public:
    explicit AbstractFace(GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> *display) : _display(display) {};
    virtual ~AbstractFace() = default;

    virtual void draw(const ScreenInfo &screenInfo) = 0;

protected:
    GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> *_display;
};

#endif //TINYWATCHY_ABSTRACTFACE_H
