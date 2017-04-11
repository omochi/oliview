#pragma once

#include "./dependency.h"
#include "./vector2.h"
#include "./size.h"

namespace oliview {
    class Rect {
    public:
        Rect();
        Rect(float x,
             float y,
             float width,
             float height);
        Rect(const Vector2 & origin,
             const Size & size);

        Vector2 origin() const;
        void set_origin(const Vector2 & value);

        Size size() const;
        void set_size(const Size & value);
    private:
        float x_;
        float y_;
        float width_;
        float height_;
    };
}
