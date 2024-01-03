#ifndef TINYWATCHY_DEFAULTFACE_H
#define TINYWATCHY_DEFAULTFACE_H

#include "AbstractFace.h"

class DefaultFace : public AbstractFace {
public:
    explicit DefaultFace(GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> *display);

    void draw(const ScreenInfo &screenInfo) override;

private:
    void drawTime(const DateTime &time);
    void drawBattery(const uint8_t &battery);
    void drawSteps(const unsigned int &steps);
    void drawMenuTitle(const std::string &title);
    void drawMenuDescription(const std::string &description);
};


#endif //TINYWATCHY_DEFAULTFACE_H
