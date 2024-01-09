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

#ifndef TINYWATCHY_NTPOPTION_H
#define TINYWATCHY_NTPOPTION_H

#include "AbstractOption.h"
#include "NTP.h"

class NTPOption : public AbstractOption {
public:
    explicit NTPOption(NTP *ntp);

    std::string getTitle() override { return "NTP Sync"; };
    std::string getDescription() override;
    void onNextButtonPressed() override {};
    void onPrevButtonPressed() override {};
    bool onSelectButtonPressed() override;
    void onBackButtonPressed() override {};

private:
    bool _synced = false;
    NTP *_ntp;
};

#endif //TINYWATCHY_NTPOPTION_H
