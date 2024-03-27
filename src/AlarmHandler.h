#ifndef TINYWATCHY_ALARMHANDLER_H
#define TINYWATCHY_ALARMHANDLER_H

#include <map>
#include "SmallRTC.h"
#include "Watchy/bma.h"
#include "ScreenInfo.h"
#include "ArduinoNvs.h"

class AlarmHandler {
public:
    struct Alarm {
        uint8_t hour{};
        uint8_t minute{};
        bool system = false;
    };

    explicit AlarmHandler(SmallRTC *smallRTC, BMA423 *accel, bool *_accelStatus, ArduinoNvs *nvs);

    void handle(ScreenInfo const *screenInfo);
    void setNextAlarm(const DateTime &time);

private:
    static uint16_t alarmTimeToIndex(uint8_t hour, uint8_t minute = 0);

    Alarm getNextAlarm(const uint16_t &currentIndex);
    void loadUserAlarm();

private:
    SmallRTC *_smallRTC;
    BMA423 *_accel;
    bool *_accelStatus;
    ArduinoNvs *_nvs;
    bool _userAlarmLoaded = false;

    std::map<uint16_t, Alarm> _alarms;
};


#endif //TINYWATCHY_ALARMHANDLER_H
