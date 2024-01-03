#ifndef TINYWATCHY_DATETIME_H
#define TINYWATCHY_DATETIME_H

#include <cstdint>

typedef struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t dayOfTheWeek; // day of week, sunday is day 1
    uint8_t day;
    uint8_t month;
    uint8_t year;   // offset from 1970;
} DateTime;

#endif //TINYWATCHY_DATETIME_H
