#ifndef TINYWATCHY_UIOPTION_H
#define TINYWATCHY_UIOPTION_H

#include "AbstractOption.h"

class UiOption : public AbstractOption {
public:
    std::string getTitle() override {
        return ">UI_test__";
    }

    std::string getDescription(const StackPage& stackPage) override {
        return "20 characters limit.";
    }

    void onNextButtonPressed() override {}

    void onPrevButtonPressed() override {}

    bool onSelectButtonPressed(const StackPage &stackPage) override { return false;}

    void onBackButtonPressed() override {}
};

#endif //TINYWATCHY_UIOPTION_H
