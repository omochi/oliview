#pragma once

#include "./dependency.h"

namespace oliview {
    enum class Axis {
        X,
        Y
    };
    
    size_t AxisToIndex(Axis axis);
    Result<Axis> AxisFromIndex(size_t index);
    
    Axis GetNextAxis(Axis axis);
}
