#include "./vector3.h"

namespace oliview {
    Vector3::Vector3(): Vector3(0, 0, 0)
    {}

    Vector3::Vector3(float x, float y, float z) {
        elements_[0] = x;
        elements_[1] = y;
        elements_[2] = z;
    }

    float Vector3::x() const {
        return elements_[0];
    }
    void Vector3::set_x(float value) {
        elements_[0] = value;
    }

    float Vector3::y() const {
        return elements_[1];
    }
    void Vector3::set_y(float value) {
        elements_[1] = value;
    }

    float Vector3::z() const {
        return elements_[2];
    }
    void Vector3::set_z(float value) {
        elements_[2] = value;
    }

    float Vector3::get(size_t index) const {
        return elements_[index];
    }

    void Vector3::set(size_t index, float value) {
        elements_[index] = value;
    }

    const float * Vector3::elements() const {
        return elements_;
    }

    float * Vector3::elements() {
        return elements_;
    }

    Vector2 Vector3::To2() const {
        return Vector2(x(), y());
    }

    Vector3 operator*(float a, const Vector3 & b) {
        return Vector3(a * b.x(), a * b.y(), a * b.z());
    }
    
    Vector3 operator/(const Vector3 & a, float b) {
        return (1.0f / b) * a;
    }

    Vector3 & operator/=(Vector3 & a, float b) {
        a = a / b;
        return a;
    }
}

