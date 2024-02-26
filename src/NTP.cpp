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

#include "NTP.h"
#include "WiFiHelper.h"
#include "NTPClient.h"

NTP::NTP(SmallRTC *smallRTC) : _smallRTC(smallRTC) {}

bool NTP::sync() {
    if (!WiFiHelper::connect()) {
        WiFiHelper::disconnect();
        Serial.println("Disconnect");
        return false;
    }

    WiFiUDP ntpUDP;
    NTPClient timeClient(ntpUDP);

    timeClient.begin();

    timeClient.setTimeOffset(TIMEZONE);
    timeClient.forceUpdate();

    time_t epochTime = timeClient.getEpochTime();

    timeClient.end();

    tmElements_t time;
    _smallRTC->BreakTime(epochTime, time);
    _smallRTC->set(time);

    return true;
}
