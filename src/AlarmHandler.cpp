#include <HTTPClient.h>
#include "AlarmHandler.h"
#include "defines_private.h"
#include "WiFiHelper.h"
#ifdef PRIVATE
#include "Private/AlarmHandlerPrivate.h"
#endif

AlarmHandler::AlarmHandler(SmallRTC *smallRTC, BMA423 *accel) : _smallRTC(smallRTC), _accel(accel) {}

void AlarmHandler::handle(ScreenInfo const *screenInfo) {
#ifdef PRIVATE
    AlarmHandlerPrivate privateHandler(_smallRTC, _accel);
    privateHandler.handle(screenInfo);
#endif
}
