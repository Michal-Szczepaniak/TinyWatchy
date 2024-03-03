#ifndef TINYWATCHY_ABOUTOPTION_H
#define TINYWATCHY_ABOUTOPTION_H

#include "AbstractOption.h"

class AboutOption : public AbstractOption {
public:
    std::string getTitle() override {
        return ">About";
    }

    std::string getDescription(const StackPage& stackPage) override {
        return "TinyWatchy v1.2";
    }

    void onNextButtonPressed() override {}

    void onPrevButtonPressed() override {}

    bool onSelectButtonPressed(const StackPage &stackPage) override { return false; }

    void onBackButtonPressed() override {}
};

#endif //TINYWATCHY_ABOUTOPTION_H
