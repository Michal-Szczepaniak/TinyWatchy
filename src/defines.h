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

#ifndef TINYWATCHY_DEFINES_H
#define TINYWATCHY_DEFINES_H

// Pins
#define RIGHT_BTN_PIN 4
#define LEFT_BTN_PIN 26
#define SELECT_BTN_PIN 35
#define BACK_BTN_PIN 25
#define DISPLAY_CS 5
#define DISPLAY_RES 9
#define DISPLAY_DC 10
#define DISPLAY_BUSY 19
#define ACC_INT_1_PIN 14
#define ACC_INT_2_PIN 12
#define VIB_MOTOR_PIN 13
#define RTC_INT_PIN 27
#define BATT_ADC_PIN 34

// HW info
#define RTC_TYPE 2 //PCF8563
#define WIFI_HOSTNAME "TinyWatchy"
#define WIFI_TIMEOUT 60

// Pin masks
#define RIGHT_BTN_MASK GPIO_SEL_4
#define LEFT_BTN_MASK GPIO_SEL_26
#define SELECT_BTN_MASK GPIO_SEL_35
#define BACK_BTN_MASK  GPIO_SEL_25
#define ACC_INT_MASK  GPIO_SEL_14

#endif //TINYWATCHY_DEFINES_H
