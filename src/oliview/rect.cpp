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
    
    Range<float> Rect::x_range() const {
        return MakeRange(origin().x(), end().x());
    }
    
    Range<float> Rect::y_range() const {
        return MakeRange(origin().y(), end().y());
    }
    
    Range<float> Rect::GetRangeFor(Axis axis) const {
        switch (axis) {
            case Axis::X:
                return x_range();
            case Axis::Y:
                return y_range();
        }
        RHETORIC_FATAL("never");
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
    
    Rect Rect::OffsetBy(const Vector2 & offset) const {
        return Rect(origin() + offset, size());
    }
    
    Rect Rect::InsetBy(const EdgeInset & inset) const {
        auto origin = this->origin();
        auto end = this->end();
        
        float top = origin.y() + inset.top();
        float left = origin.x() + inset.left();
        float bottom = end.y() - inset.bottom();
        float right = end.x() - inset.right();
        
        bottom = std::max(top, bottom);
        right = std::max(left, right);
        
        return Rect(Vector2(left, top), Size(right - left, bottom - top));
    }

    bool Rect::Contains(const Vector2 & point) const {
        return x_range().Contains(point.x()) &&
        y_range().Contains(point.y());
    }
    
    bool Rect::operator==(const Rect & other) const {
        return origin() == other.origin() &&
        size() == other.size();
    }
    
    Rect Rect::FromPoints(const Vector2 & p0,
                          const Vector2 & p1)
    {
        auto origin = p0.GetMin(p1);
        auto end = p0.GetMax(p1);
        return Rect(origin, Size(end - origin));
    }
}

