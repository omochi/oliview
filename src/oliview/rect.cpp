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
        auto origin = this->origin().ApplyTransform(m);
        auto end = this->end().ApplyTransform(m);
        return Rect(origin, Size(end - origin));
    }
    
    Rect Rect::GetUnion(const Rect & other) const {
        auto origin = this->origin().GetMin(other.origin());
        auto end = this->end().GetMax(other.end());
        return Rect(origin, Size(end - origin));
    }
    
    Rect Rect::GetIntersection(const Rect & other) const {
        auto p0 = this->end().GetMin(other.origin());
        auto p1 = this->origin().GetMax(other.end());
        auto origin = p0.GetMin(p1);
        auto end = p0.GetMax(p1);
        return Rect(origin, Size(end - origin));
    }
}

