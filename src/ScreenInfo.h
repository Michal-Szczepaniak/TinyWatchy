#ifndef TINYWATCHY_SCREENINFO_H
#define TINYWATCHY_SCREENINFO_H

#include <string>
#include "TimeLib.h"
#include "DateTime.h"

struct ScreenInfo {
    DateTime time{};
    uint32_t steps = 0;
    uint8_t battery = 0;
    std::string title;
    std::string description;
    bool suspend = false;
};

#endif //TINYWATCHY_SCREENINFO_H
