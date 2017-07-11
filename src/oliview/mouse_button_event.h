#pragma once

#include "./dependency.h"

#include "./vector2.h"

namespace oliview {
    class MouseButtonEvent {
    public:
        RHETORIC_ACCESSOR_TRIVIAL(int, button)
        RHETORIC_ACCESSOR_TRIVIAL(int, action)
        RHETORIC_ACCESSOR_TRIVIAL(int, modifier)
        RHETORIC_ACCESSOR(Vector2, pos)
    private:
        int button_;
        int action_;
        int modifier_;
        
        Vector2 pos_;
    };
}
