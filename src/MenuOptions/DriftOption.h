#ifndef TINYWATCHY_DRIFTOPTION_H
#define TINYWATCHY_DRIFTOPTION_H

#include "AbstractOption.h"
#include "NTP.h"
#include "ArduinoNvs.h"

class DriftOption : public AbstractOption {
public:
    DriftOption(NTP *ntp, ArduinoNvs *nvs) : _ntp(ntp), _nvs(nvs) {}

    std::string getTitle() override {
        return ">Drift";
    }

    std::string getDescription(const StackPage& stackPage) override {
        bool correctionInProgress = (bool) _nvs->getInt("drift_status", 0);
        return correctionInProgress ? "End correction" : "Start correction";
    }

    void onNextButtonPressed() override {}

    void onPrevButtonPressed() override {}

    bool onSelectButtonPressed(const StackPage &stackPage) override {
        bool correctionInProgress = _nvs->getInt("drift_status", 0);

        time_t ntpTime = _ntp->getTime();

        struct tm time = *localtime(&ntpTime);

        /*if (!correctionInProgress) {
            _yatchyTime->beginDrift(time, false);
        } else {
            _yatchyTime->endDrift(time, false);
            _nvs->setInt("drift", _yatchyTime->getDrift(false));
            _nvs->setInt("drift_fast", _yatchyTime->isFastDrift(false));
        }*/

        _nvs->setInt("drift_status", (int32_t)!correctionInProgress);
        _changed = true;

        return false;
    }

    void onBackButtonPressed() override {}

private:
    NTP *_ntp;
    ArduinoNvs *_nvs;
    bool _changed = false;
};


#endif //TINYWATCHY_DRIFTOPTION_H
