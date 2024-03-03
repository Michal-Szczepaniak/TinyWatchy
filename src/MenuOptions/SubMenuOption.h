#ifndef TINYWATCHY_SUBMENUOPTION_H
#define TINYWATCHY_SUBMENUOPTION_H

#include <functional>
#include <utility>
#include "AbstractOption.h"

class SubMenuOption : public AbstractOption {
public:
    SubMenuOption(std::string title, std::string description, std::function<bool()> selectCallback) :
        _title(std::move(title)), _description(std::move(description)), _selectCallback(std::move(selectCallback)) {};

    std::string getTitle() override {
        return _title;
    }

    std::string getDescription(const StackPage& stackPage) override {
        return _description;
    }

    void onNextButtonPressed() override {}

    void onPrevButtonPressed() override {}

    bool onSelectButtonPressed(const StackPage &stackPage) override {
        return _selectCallback();
    }

    void onBackButtonPressed() override {}

private:
    std::string _title;
    std::string _description;
    std::function<bool()> _selectCallback;
};

#endif //TINYWATCHY_SUBMENUOPTION_H
