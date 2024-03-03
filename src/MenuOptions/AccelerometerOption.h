#ifndef TINYWATCHY_ACCELEROMETEROPTION_H
#define TINYWATCHY_ACCELEROMETEROPTION_H

#include "AbstractOption.h"
#include "Watchy/bma.h"

class AccelerometerOption : public AbstractOption {
public:
    explicit AccelerometerOption(BMA423* accelerometer) : _accelerometer(accelerometer) {}

    std::string getTitle() override {
        return ">Accel";
    }

    std::string getDescription(const StackPage& stackPage) override {
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

    void onNextButtonPressed() override {}

    void onPrevButtonPressed() override {}

    bool onSelectButtonPressed(const StackPage &stackPage) override { return false; }

    void onBackButtonPressed() override {}

private:
    BMA423* _accelerometer;
};


#endif //TINYWATCHY_ACCELEROMETEROPTION_H
