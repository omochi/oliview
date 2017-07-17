#include "./vector2.h"

#include "./vector3.h"
#include "./matrix3x3.h"

namespace oliview {
    Vector2::Vector2(): Vector2(0, 0)
    {}

    Vector2::Vector2(float x, float y) {
        set_x(x);
        set_y(y);
    }
    
    Vector2::Vector2(const Vector2 & other) {
        *this = other;
    }
    
    Vector2 & Vector2::operator=(const Vector2 & other) {
        set_x(other.x());
        set_y(other.y());
        return *this;
    }

    float Vector2::x() const {
        return elements_[0];
    }
    void Vector2::set_x(float value) {
        elements_[0] = value;
    }

    float Vector2::y() const {
        return elements_[1];
    }
    void Vector2::set_y(float value) {
        elements_[1] = value;
    }

    float Vector2::get(size_t index) const {
        return elements_[index];
    }

    void Vector2::set(size_t index, float value) {
        elements_[index] = value;
    }

    const float * Vector2::elements() const {
        return elements_;
    }

    float * Vector2::elements() {
        return elements_;
    }

    Vector3 Vector2::To3(float z) const {
        return Vector3(x(), y(), z);
    }

    Vector2 Vector2::ApplyTransform(const Matrix3x3 & m) const {
        Vector3 x = To3(1.0) * m;
        x /= x.z();
        return x.To2();
    }
    
    Vector2 Vector2::operator-() const {
        return Vector2(-x(), -y());
    }

    Vector2 Vector2::operator+(const Vector2 & other) const {
        return Vector2(x() + other.x(),
                       y() + other.y());
    }
    
    Vector2 & Vector2::operator+=(const Vector2 & other) {
        *this = *this + other;
        return *this;
    }

    Vector2 Vector2::operator-(const Vector2 & other) const {
        return Vector2(x() - other.x(),
                       y() - other.y());
    }
    
    Vector2 & Vector2::operator-=(const Vector2 & other) {
        *this = *this - other;
        return *this;
    }
    
    Vector2 Vector2::GetMin(const Vector2 & other) const {
        return Vector2(std::min(x(), other.x()),
                       std::min(y(), other.y()));
    }
    
    Vector2 Vector2::GetMax(const Vector2 & other) const {
        return Vector2(std::max(x(), other.x()),
                       std::max(y(), other.y()));
    }

    Vector2 operator*(float a, const Vector2 & b) {
        return Vector2(a * b.x(), a * b.y());
    }
    
    Vector2 operator*(const Vector2 & a, float b) {
        return b * a;
    }
    
    Vector2 & operator*=(Vector2 & a, float b) {
        a = a * b;
        return a;
    }

    Vector2 operator/(const Vector2 & a, float b) {
        return (1.0f / b) * a;
    }
    
    Vector2 & operator/=(Vector2 & a, float b) {
        a = a / b;
        return a;
    }
}
