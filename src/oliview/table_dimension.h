#pragma once

#include "./measure_query.h"
#include "./size.h"

namespace oliview {
    enum class TableDimension {
        Row,
        Column
    };
    
    float GetSizeValueFor(const Size & size, TableDimension dimension);
    
    Option<float> GetMeasureQueryMaxValueFor(const MeasureQuery & query,
                                             TableDimension dimension);
}
