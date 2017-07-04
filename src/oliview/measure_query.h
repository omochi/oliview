#pragma once

#include "./dependency.h"

namespace oliview {
    class MeasureQuery {
    public:
        RHETORIC_ACCESSOR(Optional<float>, max_width)
        RHETORIC_ACCESSOR(Optional<float>, max_height)
    private:
        Optional<float> max_width_;
        Optional<float> max_height_;
    };
}
