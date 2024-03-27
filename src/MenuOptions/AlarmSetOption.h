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

#ifndef TINYWATCHY_ALARMSETOPTION_H
#define TINYWATCHY_ALARMSETOPTION_H

#include "AbstractOption.h"
#include "AlarmHandler.h"

class AlarmSetOption : public AbstractOption {
public:
    explicit AlarmSetOption(AlarmHandler *alarmHandler, ArduinoNvs *nvs);

    std::string getTitle() override;

    std::string getDescription(const StackPage &stackPage) override;

    void onNextButtonPressed() override;

    void onPrevButtonPressed() override;

    bool onSelectButtonPressed(const StackPage &stackPage) override;

    void onBackButtonPressed() override;

private:
    std::string formatAlarm() const;
    uint8_t getModuloForCurrentStep() const;
    uint8_t getStepSizeForCurrentStep() const;
    void loadAlarm();
    void saveAlarm();

private:
    AlarmHandler *_alarmHandler;
    ArduinoNvs *_nvs;
    struct AlarmSettings {
        uint8_t hour;
        uint8_t minute;
    } _alarm;
    RTC_DATA_ATTR static enum STEPS {
        HOURS_FIRST,
        HOURS_SECOND,
        MINUTES_FIRST,
        MINUTES_SECOND
    } _currentStep;
};


#endif //TINYWATCHY_ALARMSETOPTION_H
