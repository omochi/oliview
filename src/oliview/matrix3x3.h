#pragma once

#include "./dependency.h"
#include "./assert.h"
#include "./vector2.h"
#include "./vector3.h"

namespace oliview {
    class Rect;
    
    class Matrix3x3 {
    public:
        Matrix3x3();

        float get(int row, int column) const;
        void set(int row, int column, float value);

        const float * elements() const;
        float * elements();

        Matrix3x3 operator*(const Matrix3x3 & other) const;

        static Matrix3x3 Identity();
        static Matrix3x3 Translation(const Vector2 & translation);
        static Matrix3x3 Rotation(float radian);
        static Matrix3x3 Scaling(const Vector2 & scaling);
        static Matrix3x3 RectTransform(const Rect & from,
                                       const Rect & to);
    private:
        float elements_[9];
    };

    Vector3 operator*(const Vector3 & a, const Matrix3x3 & b);
}
