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

#ifndef TINYWATCHY_ABSTRACTOPTION_H
#define TINYWATCHY_ABSTRACTOPTION_H

#include <string>

class AbstractOption {
public:
    virtual ~AbstractOption() = default;

    virtual std::string getTitle() = 0;
    virtual std::string getDescription() = 0;
    virtual void onNextButtonPressed() = 0;
    virtual void onPrevButtonPressed() = 0;
    virtual bool onSelectButtonPressed() = 0;
    virtual void onBackButtonPressed() = 0;
};

#endif //TINYWATCHY_ABSTRACTOPTION_H
