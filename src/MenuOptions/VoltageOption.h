#ifndef TINYWATCHY_VOLTAGEOPTION_H
#define TINYWATCHY_VOLTAGEOPTION_H

#include <ios>
#include <sstream>
#include <esp32-hal-adc.h>
#include "AbstractOption.h"
#include "defines.h"

class VoltageOption : public AbstractOption {
public:
    std::string getTitle() override {
        return ">Voltage";
    }

    std::string getDescription(const StackPage& stackPage) override {
        float voltage = static_cast<float>(analogReadMilliVolts(BATT_ADC_PIN)) / 1000.0f * 2.0f;
        std::ostringstream out;
        out.precision(2);
        out << std::fixed << voltage;
        return out.str() + " V";
    }

    void onNextButtonPressed() override {}

    void onPrevButtonPressed() override {}

    bool onSelectButtonPressed(const StackPage &stackPage) override { return false;}

    void onBackButtonPressed() override {}
};


#endif //TINYWATCHY_VOLTAGEOPTION_H
