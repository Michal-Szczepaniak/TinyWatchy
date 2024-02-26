#ifndef TINYWATCHY_ALARMHANDLER_H
#define TINYWATCHY_ALARMHANDLER_H

#include "SmallRTC.h"
#include "Watchy/bma.h"
#include "ScreenInfo.h"

class AlarmHandler {
public:
    explicit AlarmHandler(SmallRTC *smallRTC, BMA423 *accel);

    void handle(ScreenInfo const *screenInfo);

private:
    SmallRTC *_smallRTC;
    BMA423 *_accel;
};


#endif //TINYWATCHY_ALARMHANDLER_H
