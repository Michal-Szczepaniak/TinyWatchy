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

#include "NTPOption.h"

NTPOption::NTPOption(NTP *ntp) : _ntp(ntp) {
}

std::string NTPOption::getDescription() {
    std::string message = _error ? "Error syncing" : _synced ? "Synced" : "Sync time via NTP";

    _synced = false;
    _error = false;

    return message;
}

bool NTPOption::onSelectButtonPressed() {
    if (_synced) return false;

    if (!_ntp->sync()) {
        _error = true;
    } else {
        _synced = true;
    }

    return false;
}
