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
    
    Vector2 Size::ToVector() const {
        return Vector2(width_, height_);
    }
}
