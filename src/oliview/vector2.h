#pragma once

#include "./dependency.h"

namespace oliview {
    class Vector3;
    class Matrix3x3;

    class Vector2 {
    public:
        Vector2();
        Vector2(float x, float y);
        Vector2(const Vector2 & other);
        Vector2 & operator=(const Vector2 & other);

        float x() const;
        void set_x(float value);

        float y() const;
        void set_y(float value);

        float get(size_t index) const;
        void set(size_t index, float value);

        const float * elements() const;
        float * elements();

        Vector3 To3(float z) const;

        Vector2 ApplyTransform(const Matrix3x3 & m) const;

        Vector2 operator-() const;
        
        Vector2 operator+(const Vector2 & other) const;
        Vector2 & operator+=(const Vector2 & other);
        Vector2 operator-(const Vector2 & other) const;
        Vector2 & operator-=(const Vector2 & other);
        
        Vector2 GetMin(const Vector2 & other) const;
        Vector2 GetMax(const Vector2 & other) const;
    private:
        float elements_[2];
    };

    Vector2 operator*(float a, const Vector2 & b);
    Vector2 operator*(const Vector2 & a, float b);
    Vector2 & operator*=(Vector2 & a, float b);
    Vector2 operator/(const Vector2 & a, float b);
    Vector2 & operator/=(Vector2 & a, float b);
}
