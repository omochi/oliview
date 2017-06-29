#pragma once

#include "./dependency.h"
#include "./size.h"
#include "./vector2.h"
#include "./matrix3x3.h"

namespace oliview {
    class Rect {
    public:
        Rect();
        Rect(const Vector2 & origin,
             const Size & size);

        RHETORIC_ACCESSOR(Vector2, origin)
        RHETORIC_ACCESSOR(Size, size)

        Vector2 center() const;

        Vector2 end() const;

        Rect ApplyTransform(const Matrix3x3 & m) const;
        
        Rect GetUnion(const Rect & other) const;
        Rect GetIntersection(const Rect & other) const;
    private:
        Vector2 origin_;
        Size size_;
    };
}
