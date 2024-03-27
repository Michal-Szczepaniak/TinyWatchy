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

#include "AlarmSetOption.h"

enum AlarmSetOption::STEPS AlarmSetOption::_currentStep = AlarmSetOption::HOURS_FIRST;

AlarmSetOption::AlarmSetOption(AlarmHandler *alarmHandler, ArduinoNvs *nvs) : _alarmHandler(alarmHandler), _nvs(nvs),
    _alarm({0, 0}) {
}

std::string AlarmSetOption::getTitle() {
    return ">Set alarm";
}

std::string AlarmSetOption::getDescription(const StackPage &stackPage) {
    if (stackPage.selected) {
        loadAlarm();
        return formatAlarm();
    } else {
        return "Set alarm";
    }
}

void AlarmSetOption::onNextButtonPressed() {
    uint8_t modulo = getModuloForCurrentStep();
    uint8_t stepSize = getStepSizeForCurrentStep();

    loadAlarm();

    if (_currentStep == HOURS_FIRST || _currentStep == HOURS_SECOND) {
        if (_alarm.hour + stepSize > modulo) {
            _alarm.hour = 0;
        } else {
            _alarm.hour += stepSize;
        }
    } else {
        if (_alarm.minute + stepSize > modulo) {
            _alarm.minute = 0;
        } else {
            _alarm.minute = (_alarm.minute + stepSize) % modulo;
        }
    }

    saveAlarm();
}

void AlarmSetOption::onPrevButtonPressed() {
    uint8_t modulo = getModuloForCurrentStep();
    uint8_t stepSize = getStepSizeForCurrentStep();

    loadAlarm();

    if (_currentStep == HOURS_FIRST || _currentStep == HOURS_SECOND) {
        if (_alarm.hour == 0) {
            _alarm.hour = modulo-1;
        } else {
            _alarm.hour -= stepSize;
        }
    } else {
        if (_alarm.minute == 0) {
            _alarm.minute = modulo-1;
        } else {
            _alarm.minute -= stepSize;
        }
    }

    saveAlarm();
}

bool AlarmSetOption::onSelectButtonPressed(const StackPage &stackPage) {
    loadAlarm();

    if (!stackPage.selected) {
        _currentStep = HOURS_FIRST;
        return true;
    } else {
        _currentStep = static_cast<STEPS>((_currentStep + 1) % 4);
        return false;
    }
}

void AlarmSetOption::onBackButtonPressed() {
}

std::string AlarmSetOption::formatAlarm() const {
    std::string result;

    if (_alarm.hour < 10)
        result += "0";

    result += std::to_string(_alarm.hour);

    result += ":";

    if (_alarm.minute < 10)
        result += "0";

    result += std::to_string(_alarm.minute);

    uint8_t insertPosition = _currentStep == HOURS_FIRST ? 0 :
                                _currentStep == HOURS_SECOND ? 1 :
                                    _currentStep == MINUTES_FIRST ? 3 :
                                        _currentStep == MINUTES_SECOND ? 4 : 0;

    result.insert(insertPosition, ">");

    return result;
}

uint8_t AlarmSetOption::getModuloForCurrentStep() const {
    switch (_currentStep) {
        case HOURS_FIRST:
        case HOURS_SECOND:
            return 24;
        case MINUTES_FIRST:
        case MINUTES_SECOND:
            return 60;
    }
}

uint8_t AlarmSetOption::getStepSizeForCurrentStep() const {
    switch (_currentStep) {
        case HOURS_FIRST:
        case MINUTES_FIRST:
            return 10;
        case HOURS_SECOND:
        case MINUTES_SECOND:
            return 1;
    }
}

void AlarmSetOption::loadAlarm() {
    std::vector<uint8_t> alarmTime = _nvs->getBlob("alarm");
    if (!alarmTime.empty()) {
        _alarm.hour = alarmTime[0];
        _alarm.minute = alarmTime[1];
    }
}

void AlarmSetOption::saveAlarm() {
    union {
        AlarmSettings alarm;
        uint8_t alarmArray[2];
    } alarmUnion = {
        .alarm = _alarm
    };

    _nvs->setBlob("alarm", alarmUnion.alarmArray, sizeof(uint8_t)*2);
}
