#include "./rect.h"

namespace oliview {
    Rect::Rect():
    Rect(Vector2(), Size())
    {}

    Rect::Rect(const Vector2 & origin,
               const Size & size):
    origin_(origin),
    size_(size)
    {}

    Vector2 Rect::center() const {
        return origin_ + 0.5f * size_.ToVector();
    }

    Vector2 Rect::end() const {
        return origin_ + size_.ToVector();
    }

    Rect Rect::ApplyTransform(const Matrix3x3 & m) const {
        auto p0 = origin().ApplyTransform(m);
        auto p1 = end().ApplyTransform(m);
        return Rect::FromPoints(p0, p1);
    }
    
    Rect Rect::GetUnion(const Rect & other) const {
        auto p0 = origin().GetMin(other.origin());
        auto p1 = end().GetMax(other.end());
        return Rect::FromPoints(p0, p1);
    }
    
    Rect Rect::GetIntersection(const Rect & other) const {
        auto p0 = origin().GetMax(other.origin());
        auto p1 = end().GetMin(other.end());
        return Rect::FromPoints(p0, p1);
    }
    
    Rect Rect::FromPoints(const Vector2 & p0,
                          const Vector2 & p1)
    {
        auto origin = p0.GetMin(p1);
        auto end = p0.GetMax(p1);
        return Rect(origin, Size(end - origin));
    }
}

