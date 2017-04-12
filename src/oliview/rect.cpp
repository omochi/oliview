#include "./rect.h"

namespace oliview {
    Rect::Rect():
    Rect(0, 0, 0, 0)
    {}

    Rect::Rect(float x,
               float y,
               float width,
               float height):
    origin_(x, y),
    size_(width, height)
    {}

    Rect::Rect(const Vector2 & origin,
               const Vector2 & size):
    Rect(origin.x(), origin.y(), size.x(), size.y())
    {}

    Vector2 Rect::origin() const {
        return origin_;
    }

    void Rect::set_origin(const Vector2 & value) {
        origin_ = value;
    }

    Vector2 Rect::size() const {
        return size_;
    }

    void Rect::set_size(const Vector2 & value) {
        size_ = value;
    }

    Vector2 Rect::center() const {
        return origin_ + 0.5f * size_;
    }

    Vector2 Rect::end() const {
        return origin_ + size_;
    }

    Rect Rect::ApplyTransform(const Matrix3x3 & m) const {
        auto ret_origin = origin_.ApplyTransform(m);
        auto ret_end = end().ApplyTransform(m);
        return Rect(ret_origin, ret_end - ret_origin);
    }
}

