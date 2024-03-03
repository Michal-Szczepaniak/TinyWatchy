#ifndef TINYWATCHY_STACKPAGE_H
#define TINYWATCHY_STACKPAGE_H

struct StackPage {
    uint8_t pageIndex = 0;
    bool selected = false;
    uint8_t itemIndex = 0;
};

#endif //TINYWATCHY_STACKPAGE_H
