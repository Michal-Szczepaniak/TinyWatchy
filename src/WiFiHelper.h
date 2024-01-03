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

#ifndef MIHAU_TEMP_WIFIHELPER_H
#define MIHAU_TEMP_WIFIHELPER_H

#include <WiFi.h>
#include "defines.h"
#include "defines_private.h"

class WiFiHelper {
public:
    static bool connect() {
        if (WiFiClass::getMode() != WIFI_STA) {
            WiFiClass::mode(WIFI_STA);
        }

        WiFi.setAutoReconnect(true);
        WiFiClass::setHostname(WIFI_HOSTNAME);
        WiFi.begin(WIFI_SSID, WIFI_PASS);
        if (WiFi.waitForConnectResult(WIFI_TIMEOUT) != WL_CONNECTED) {
            return false;
        }

        return true;
    }

    static void disconnect() {
        if (WiFiClass::getMode() != WIFI_OFF) {
            WiFiClass::mode(WIFI_OFF);
            WiFi.disconnect(true);
        }
    }

    static bool isConnected() {
        return WiFiClass::status() == WL_CONNECTED;
    }
};


#endif //MIHAU_TEMP_WIFIHELPER_H
