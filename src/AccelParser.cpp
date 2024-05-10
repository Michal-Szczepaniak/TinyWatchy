#include "AccelParser.h"

AccelParser::Vector3 AccelParser::normalizeAccel(const Accel &data) {
    Vector3 vec3;

    vec3.x = round((std::clamp((float)data.x, -1100.f, 1100.f)/12.2222222f)*100.f)/100.f;
    vec3.y = round((std::clamp((float)data.y, -1100.f, 1100.f)/12.2222222f)*100.f)/100.f;
    vec3.z = round((std::clamp((float)data.z, -1100.f, 1100.f)/12.2222222f)*100.f)/100.f;

    return vec3;
}

AccelParser::Orientation AccelParser::normalizeOrientation(const AccelParser::Vector3 &vec3) {
    if (isWithinRange(vec3.x, 0) && isWithinRange(vec3.y, 0) && isWithinRange(vec3.z, -90)) {
        return FACE_UP;
    } else if (isWithinRange(vec3.x, 0) && isWithinRange(vec3.y, 0) && isWithinRange(vec3.z, 90)) {
        return FACE_DOWN;
    } else if (isWithinRange(vec3.x, 90) && isWithinRange(vec3.y, 0) && isWithinRange(vec3.z, 0)) {
        return BOTTOM_EDGE;
    } else if (isWithinRange(vec3.x, -90) && isWithinRange(vec3.y, 0) && isWithinRange(vec3.z, 0)) {
        return TOP_EDGE;
    } else if (isWithinRange(vec3.x, 0) && isWithinRange(vec3.y, 90) && isWithinRange(vec3.z, 0)) {
        return LEFT_EDGE;
    } else if (isWithinRange(vec3.x, 0) && isWithinRange(vec3.y, -90) && isWithinRange(vec3.z, 0)) {
        return RIGHT_EDGE;
    }

    return FACE_UP;
}

AccelParser::Orientation AccelParser::normalizeOrientation(const Accel &data) {
    return normalizeOrientation(normalizeAccel(data));
}

bool AccelParser::isWithinRange(float angle, float centerPoint) {
    return angle < (centerPoint + 45.f) && angle > (centerPoint - 45.f);
}
