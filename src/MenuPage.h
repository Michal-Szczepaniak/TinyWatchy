#ifndef TINYWATCHY_MENUPAGE_H
#define TINYWATCHY_MENUPAGE_H

#include "MenuOptions/AbstractOption.h"

struct MenuPage {
    std::vector<AbstractOption*> items;
};

#endif //TINYWATCHY_MENUPAGE_H
