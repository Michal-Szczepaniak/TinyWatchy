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

#ifndef TINYWATCHY_DATETIME_H
#define TINYWATCHY_DATETIME_H

#include <cstdint>

typedef struct DateTime {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t dayOfTheWeek; // day of week, sunday is day 1
    uint8_t day;
    uint8_t month;
    uint8_t year;   // offset from 1970;

    DateTime operator-(const DateTime& other) const
    {
        DateTime result;

        if (hour < other.hour) {
            result.hour = (hour + 24) - other.hour;
        } else {
            result.hour = hour - other.hour;
        }

        if (minute < other.minute) {
            result.minute = (minute + 60) - other.minute;
        } else {
            result.minute = minute - other.minute;
        }

        return result;
    }
} DateTime;

#endif //TINYWATCHY_DATETIME_H
