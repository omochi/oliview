#include "./rect.h"

namespace oliview {
    Rect::Rect():
    Rect(0, 0, 0, 0)
    {}

    Rect::Rect(float x,
               float y,
               float width,
               float height):
    x_(x),
    y_(y),
    width_(width),
    height_(height)
    {}

    Rect::Rect(const Vector2 & origin,
               const Size & size):
    Rect(origin.x(), origin.y(), size.width(), size.height())
    {}

    Vector2 Rect::origin() const {
        return Vector2(x_, y_);
    }

    void Rect::set_origin(const Vector2 & value) {
        x_ = value.x();
        y_ = value.y();
    }

    Size Rect::size() const {
        return Size(width_, height_);
    }

    void Rect::set_size(const Size & value) {
        width_ = value.width();
        height_ = value.height();
    }
}

