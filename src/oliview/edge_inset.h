#pragma once

#include "./dependency.h"

namespace oliview {
    class EdgeInset {
    public:
        EdgeInset();
        EdgeInset(float top, float left, float bottom, float right);
        
        RHETORIC_ACCESSOR_TRIVIAL(float, top)
        RHETORIC_ACCESSOR_TRIVIAL(float, left)
        RHETORIC_ACCESSOR_TRIVIAL(float, bottom)
        RHETORIC_ACCESSOR_TRIVIAL(float, right)
    private:
        float top_;
        float left_;
        float bottom_;
        float right_;
    };
}
