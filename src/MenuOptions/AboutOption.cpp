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

#include <sstream>
#include "AboutOption.h"
#include "Watchy/bma.h"
#include "defines.h"

uint8_t AboutOption::_option = 0;

AboutOption::AboutOption(BMA423 *accelerometer) : _accelerometer(accelerometer) {
}

std::string AboutOption::getDescription() {
    switch (_option) {
        case 1:
        {
            Accel _data;

            if (!_accelerometer->getAccel(_data)) {
                return "Couldn't get accelerometer data";
            }

            std::string text = "X: ";
            text.append(std::to_string(_data.x));
            text.append(" Y: ");
            text.append(std::to_string(_data.y));
            text.append(" Z: ");
            text.append(std::to_string(_data.z));

            return text;
        }
        case 2:
        {
            float voltage = static_cast<float>(analogReadMilliVolts(BATT_ADC_PIN)) / 1000.0f * 2.0f;
            std::ostringstream out;
            out.precision(2);
            out << std::fixed << voltage;
            return out.str() + " V";
        }
        default:
            return "TinyWatchy v1.0";
    }
}

void AboutOption::onNextButtonPressed() {
    _option++;

    if (_option == 3) {
        _option = 0;
    }
}

void AboutOption::onPrevButtonPressed() {
    if (_option == 0) {
        _option = 2;
    } else {
        _option--;
    }
}

bool AboutOption::onSelectButtonPressed() {
    return !(*_level);
}

void AboutOption::onBackButtonPressed() {
    _option = 0;
}
