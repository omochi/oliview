#pragma once

#include "./dependency.h"

#include "./vector2.h"

namespace oliview {
    enum class MouseEventType {
        Down,
        Move,
        Up,
        Cancel
    };
    
    class MouseEvent {
    public:
        RHETORIC_ACCESSOR_TRIVIAL(MouseEventType, type)
        RHETORIC_ACCESSOR(Vector2, pos)
        RHETORIC_ACCESSOR_TRIVIAL(Optional<int>, button)
        RHETORIC_ACCESSOR_TRIVIAL(Optional<int>, modifier)
    private:
        MouseEventType type_;
        Vector2 pos_;
        Optional<int> button_;
        Optional<int> modifier_;
    };
}
