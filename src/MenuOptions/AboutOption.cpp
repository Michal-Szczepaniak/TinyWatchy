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
#include "Faces/UwUFace.h"
#include "Faces/DefaultFace.h"

uint8_t AboutOption::_option = 0;
const uint8_t AboutOption::MAX_OPTION = 3;

AboutOption::AboutOption(BMA423 *accelerometer, Screen *screen) : _accelerometer(accelerometer), _screen(screen) {
    _nvs.begin();
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
        case 3:
            if (*_level < 2) {
                return "Change watchface";
            } else {
                int64_t watchface = _nvs.getInt("watchface", 0);
                return _screen->getFaces().at(watchface)->getName();
            }
        default:
            return "TinyWatchy v1.0";
    }
}

void AboutOption::onNextButtonPressed() {
    if (*_level < 2) {
        _option++;

        if (_option == MAX_OPTION + 1) {
            _option = 0;
        }
    } else {
        if (_option == 3) {
            int64_t watchface = _nvs.getInt("watchface", 0);
            uint32_t watchFaceCount = _screen->getFaces().size();
            _nvs.setInt("watchface", watchface % watchFaceCount);
        }
    }
}

void AboutOption::onPrevButtonPressed() {
    if (*_level < 2) {
        if (_option == 0) {
            _option = MAX_OPTION;
        } else {
            _option--;
        }
    } else {
        if (_option == 3) {
            int64_t watchface = _nvs.getInt("watchface", 0);
            uint32_t watchFaceCount = _screen->getFaces().size();
            _nvs.setInt("watchface", watchface - 1 < 0 ? watchFaceCount - 1 : watchface - 1);
        }
    }
}

bool AboutOption::onSelectButtonPressed() {
    return (!(*_level)) || (_option == 3);
}

void AboutOption::onBackButtonPressed() {
    if (*_level < 2)
        _option = 0;
}
