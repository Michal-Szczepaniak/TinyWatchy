#ifndef TINYWATCHY_ACCELEROMETEROPTION_H
#define TINYWATCHY_ACCELEROMETEROPTION_H

#include <sstream>
#include "AbstractOption.h"
#include "Watchy/bma.h"
#include "AccelParser.h"

class AccelerometerOption : public AbstractOption {
public:
    explicit AccelerometerOption(BMA423* accelerometer) : _accelerometer(accelerometer) {}

    std::string getTitle() override {
        return ">Accel";
    }

    std::string getDescription(const StackPage& stackPage) override {
        Accel data;

        if (!_accelerometer->getAccel(data)) {
            return "Couldn't get accelerometer data";
        }

        AccelParser::Vector3 angles = AccelParser::normalizeAccel(data);

        std::ostringstream text;
        text.precision(2);
        text
            << "X: "
            << angles.x
            << "° Y: "
            << angles.y
            << "° Z: "
            << angles.z
            << "°"
        ;

        return text.str();
    }

    void onNextButtonPressed() override {}

    void onPrevButtonPressed() override {}

    bool onSelectButtonPressed(const StackPage &stackPage) override { return false; }

    void onBackButtonPressed() override {}

private:
    BMA423* _accelerometer;
};


#endif //TINYWATCHY_ACCELEROMETEROPTION_H
