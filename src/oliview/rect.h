#pragma once

#include "./dependency.h"
#include "./vector2.h"
#include "./matrix3x3.h"

namespace oliview {
    class Rect {
    public:
        Rect();
        Rect(float x,
             float y,
             float width,
             float height);
        Rect(const Vector2 & origin,
             const Vector2 & size);

        Vector2 origin() const;
        void set_origin(const Vector2 & value);

        Vector2 size() const;
        void set_size(const Vector2 & value);

        Vector2 center() const;

        Vector2 end() const;

        Rect ApplyTransform(const Matrix3x3 & m) const;
    private:
        Vector2 origin_;
        Vector2 size_;
    };
}
