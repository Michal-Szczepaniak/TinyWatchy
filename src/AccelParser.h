#ifndef TINYWATCHY_ACCELPARSER_H
#define TINYWATCHY_ACCELPARSER_H

#include <Arduino.h>
#include "Watchy/bma.h"

class AccelParser {
public:
    struct Vector3 {
        float x = 0;
        float y = 0;
        float z = 0;
    };

    enum Orientation {
        FACE_UP,
        FACE_DOWN,
        LEFT_EDGE,
        RIGHT_EDGE,
        TOP_EDGE,
        BOTTOM_EDGE,
    };

    static Vector3 normalizeAccel(const Accel &data);

    static Orientation normalizeOrientation(const Vector3 &vec3);
    static Orientation normalizeOrientation(const Accel &data);

private:
    static bool isWithinRange(float angle, float centerPoint);
};


#endif //TINYWATCHY_ACCELPARSER_H
