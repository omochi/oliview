#include "./matrix3x3.h"

#include "./rect.h"

namespace oliview {
    Matrix3x3::Matrix3x3() {
        for (int i = 0; i < RHETORIC_ARRAY_SIZE(elements_); i++) {
            elements_[i] = 0;
        }
    }
    
    float Matrix3x3::get(int row, int column) const {
        return elements_[row * 3 + column];
    }

    void Matrix3x3::set(int row, int column, float value) {
        elements_[row * 3 + column] = value;
    }

    const float * Matrix3x3::elements() const {
        return elements_;
    }

    float * Matrix3x3::elements() {
        return elements_;
    }

    Matrix3x3 Matrix3x3::operator*(const Matrix3x3 & other) const {
        Matrix3x3 ret;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                float x = 0;
                for (int k = 0; k < 3; k++) {
                    x += get(i, k) * other.get(k, j);
                }
                ret.set(i, j, x);
            }
        }
        return ret;
    }

    Matrix3x3 & Matrix3x3::operator*=(const Matrix3x3 & other) {
        *this = (*this) * other;
        return *this;
    }

    Matrix3x3 Matrix3x3::Transpose() const {
        Matrix3x3 ret;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                ret.set(i, j, get(j, i));
            }
        }
        return ret;
    }

    Matrix3x3 Matrix3x3::Identity() {
        Matrix3x3 ret;
        ret.set(0, 0, 1);
        ret.set(1, 1, 1);
        ret.set(2, 2, 1);
        return ret;
    }

    Matrix3x3 Matrix3x3::Translation(const Vector2 & translation) {
        Matrix3x3 ret;
        ret.set(0, 0, 1);
        ret.set(1, 1, 1);
        ret.set(2, 0, translation.x());
        ret.set(2, 1, translation.y());
        ret.set(2, 2, 1);
        return ret;
    }

    Matrix3x3 Matrix3x3::Rotation(float radian) {
        float c = cosf(radian);
        float s = sinf(radian);
        Matrix3x3 ret;
        ret.set(0, 0, c);
        ret.set(1, 0, -s);
        ret.set(0, 1, s);
        ret.set(1, 1, c);
        ret.set(2, 2, 1);
        return ret;
    }

    Matrix3x3 Matrix3x3::Scaling(const Vector2 & scaling) {
        Matrix3x3 ret;
        ret.set(0, 0, scaling.x());
        ret.set(1, 1, scaling.y());
        ret.set(2, 2, 1);
        return ret;
    }

    Vector3 operator*(const Vector3 & a, const Matrix3x3 & b) {
        Vector3 ret;
        for (int i = 0; i < 3; i++) {
            float x = 0;
            for (int j = 0; j < 3; j++) {
                x += a.get(j) * b.get(j, i);
            }
            ret.set(i, x);
        }
        return ret;
    }

    Vector3 & operator*=(Vector3 & a, const Matrix3x3 & b) {
        a = a * b;
        return a;
    }

}
