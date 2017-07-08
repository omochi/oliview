#pragma once

#include "./dependency.h"

namespace oliview {
    float ToRadian(float degree);
    float ToDegree(float radian);
    
    template <typename T>
    T Clamp(const T & value, const T & min, const T & max);
}

#include "./math_inline.h"
