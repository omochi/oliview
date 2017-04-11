#include "./size.h"

namespace oliview {
    Size::Size(): Size(0, 0)
    {}

    Size::Size(float width, float height):
    width_(width),
    height_(height)
    {}

    float Size::width() const {
        return width_;
    }

    void Size::set_width(float value) {
        width_ = value;
    }

    float Size::height() const {
        return height_;
    }

    void Size::set_height(float value) {
        height_ = value;
    }
}
