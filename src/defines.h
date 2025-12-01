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
#define RIGHT_BTN_PIN  1
#define LEFT_BTN_PIN   0
#define SELECT_BTN_PIN 2
#define BACK_BTN_PIN   3
#define DISPLAY_CS     8
#define DISPLAY_RES    2
#define DISPLAY_DC     3
#define DISPLAY_BUSY   4
#define SPI_SCK        7
#define SPI_MISO       (-1)
#define SPI_MOSI       5
#define SPI_SS         (-1)

#define ACC_INT_1_PIN 4
#define ACC_INT_2_PIN 5

#define CHARGING_STATUS_PIN 6

#define VIB_MOTOR_PIN 13

#define BATT_ADC_PIN 6

// HW info
#define WIFI_HOSTNAME "TinyYatchy"
#define WIFI_TIMEOUT 60000

#endif //TINYWATCHY_DEFINES_H
