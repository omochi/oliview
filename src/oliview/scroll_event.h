#pragma once

#include "./dependency.h"
#include "./vector2.h"

namespace oliview {
    class ScrollEvent {
    public:
        RHETORIC_ACCESSOR(Vector2, scroll)
        RHETORIC_ACCESSOR(Vector2, pos)
    private:
        Vector2 scroll_;
        Vector2 pos_;
    };
}
