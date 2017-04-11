#pragma once

#include "./dependency.h"

namespace oliview {
    class Vector2 {
    public:
        Vector2();
        Vector2(float x, float y);

        float x() const;
        void set_x(float value);

        float y() const;
        void set_y(float value);

        const float * elements() const;
        float * elements();
    private:
        float elements_[2];
    };
}
