#include "./math.h"

namespace oliview {

    float ToRadian(float degree) {
        return (float)(degree * M_PI / 180.0);
    }

    float ToDegree(float radian) {
        return (float)(radian * 180.0f / M_PI);
    }
    

}
