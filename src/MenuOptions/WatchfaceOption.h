#ifndef TINYWATCHY_WATCHFACEOPTION_H
#define TINYWATCHY_WATCHFACEOPTION_H

class WatchfaceOption : public AbstractOption {
public:
    WatchfaceOption(Screen *screen, ArduinoNvs *nvs) : _screen(screen), _nvs(nvs) {}

    std::string getTitle() override {
        return ">Watchface";
    }

    std::string getDescription(const StackPage& stackPage) override {
        int64_t watchface = _nvs->getInt("watchface", 0);
        return _screen->getFaces().at(watchface)->getName();
    }

    void onNextButtonPressed() override {}

    void onPrevButtonPressed() override {}

    bool onSelectButtonPressed(const StackPage &stackPage) override {
        int64_t watchface = _nvs->getInt("watchface", 0);
        uint32_t watchFaceCount = _screen->getFaces().size();
        _nvs->setInt("watchface", (watchface + 1) % watchFaceCount);
        return false;
    }

    void onBackButtonPressed() override {}

private:
    Screen *_screen;
    ArduinoNvs *_nvs;
};

#endif //TINYWATCHY_WATCHFACEOPTION_H
