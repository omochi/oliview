#include "./size.h"

namespace oliview {
    Size::Size():
    Size(0, 0)
    {}
    
    Size::Size(float width,
               float height):
    width_(width),
    height_(height)
    {}
    
    Size::Size(const Vector2 & vector):
    Size(vector.x(), vector.y())
    {}
    
    bool Size::operator==(const Size & other) const {
        return width() == other.width() &&
        height() == other.height();
    }
    
    Size Size::GetMin(const Size & other) const {
        return Size(std::min(width(), other.width()),
                    std::min(height(), other.height()));
    }
    
    Size Size::GetMax(const Size & other) const {
        return Size(std::max(width(), other.width()),
                    std::max(height(), other.height()));
    }
    
    Vector2 Size::ToVector() const {
        return Vector2(width_, height_);
    }
}
