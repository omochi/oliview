#include "./vector2.h"

namespace oliview {
    Vector2::Vector2(): Vector2(0, 0)
    {}

    Vector2::Vector2(float x, float y) {
        elements_[0] = x;
        elements_[1] = y;
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

    const float * Vector2::elements() const {
        return elements_;
    }

    float * Vector2::elements() {
        return elements_;
    }
}
