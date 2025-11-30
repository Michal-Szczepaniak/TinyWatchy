#ifndef TINYWATCHY_FLASHINGMODEOPTION_H
#define TINYWATCHY_FLASHINGMODEOPTION_H

#include <ios>
#include <sstream>
#include <esp32-hal-adc.h>
#include "AbstractOption.h"
#include "defines.h"

class FlashingModeOption : public AbstractOption {
public:
    std::string getTitle() override {
        return ">Flashing";
    }

    std::string getDescription(const StackPage& stackPage) override {
        return "Enable flashing";
    }

    void onNextButtonPressed() override {}

    void onPrevButtonPressed() override {}

    bool onSelectButtonPressed(const StackPage &stackPage) override {
        delay(20000);
        return false;
    }

    void onBackButtonPressed() override {}
};


#endif //TINYWATCHY_FLASHINGMODEOPTION_H
