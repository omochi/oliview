#pragma once

#include "./dependency.h"

namespace oliview {
    class MeasureQuery {
    public:
        RHETORIC_ACCESSOR(Option<float>, max_width)
        RHETORIC_ACCESSOR(Option<float>, max_height)
    private:
        Option<float> max_width_;
        Option<float> max_height_;
    };
}
