#pragma once

#include "./dependency.h"
#include "./vector2.h"

namespace oliview {
    class Vector3 {
    public:
        Vector3();
        Vector3(float x, float y, float z);

        float x() const;
        void set_x(float value);

        float y() const;
        void set_y(float value);

        float z() const;
        void set_z(float value);

        float get(size_t index) const;
        void set(size_t index, float value);

        const float * elements() const;
        float * elements();

        Vector2 To2() const;
    private:
        float elements_[3];
    };

    Vector3 operator*(float a, const Vector3 & b);

    Vector3 operator/(const Vector3 & a, float b);
    Vector3 & operator/=(Vector3 & a, float b);
}
