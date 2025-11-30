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

bool NTP::sync() {
    time_t epochTime = getTime();
    Serial.printf("Epoch time: %lld\n", epochTime);

    if (epochTime == 0) {
        return false;
    }

    struct tm time = YatchyTime::getLocalTimeFromTimeT(epochTime);
    YatchyTime::setTime(time);

    return true;
}

time_t NTP::getTime() {
    if (!WiFiHelper::connect()) {
        WiFiHelper::disconnect();
        return 0;
    }

    WiFiUDP ntpUDP;
    NTPClient timeClient(ntpUDP);

    timeClient.begin();

    timeClient.forceUpdate();

    timeClient.end();

    return timeClient.getEpochTime();
}
