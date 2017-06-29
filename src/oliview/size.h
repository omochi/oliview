#pragma once

#include "./dependency.h"
#include "./vector2.h"

namespace oliview {
    class Size {
    public:
        Size();
        Size(float width,
             float height);
        explicit Size(const Vector2 & vector);
        
        RHETORIC_ACCESSOR_TRIVIAL(float, width)
        RHETORIC_ACCESSOR_TRIVIAL(float, height)
        
        Vector2 ToVector() const;
    private:
        float width_;
        float height_;
    };
}
