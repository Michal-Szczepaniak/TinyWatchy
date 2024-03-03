#ifndef TINYWATCHY_DRIFTOPTION_H
#define TINYWATCHY_DRIFTOPTION_H

#include "AbstractOption.h"
#include "SmallRTC.h"
#include "NTP.h"
#include "ArduinoNvs.h"

class DriftOption : public AbstractOption {
public:
    DriftOption(NTP *ntp, SmallRTC *smallRTC, ArduinoNvs *nvs) : _ntp(ntp), _smallRTC(smallRTC), _nvs(nvs) {}

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

        tmElements_t time;
        _smallRTC->BreakTime(ntpTime, time);

        if (!correctionInProgress) {
            _smallRTC->beginDrift(time, false);
        } else {
            _smallRTC->endDrift(time, false);
            _nvs->setInt("drift", _smallRTC->getDrift(false));
            _nvs->setInt("drift_fast", _smallRTC->isFastDrift(false));
        }

        _nvs->setInt("drift_status", !correctionInProgress);
        _changed = true;

        return false;
    }

    void onBackButtonPressed() override {}

private:
    NTP *_ntp;
    SmallRTC *_smallRTC;
    ArduinoNvs *_nvs;
    bool _changed = false;
};


#endif //TINYWATCHY_DRIFTOPTION_H
