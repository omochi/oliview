#include "./table_dimension.h"

namespace oliview {
    float GetSizeValueFor(const Size & size, TableDimension dimension)
    {
        switch (dimension) {
            case TableDimension::Row:
                return size.height();
            case TableDimension::Column:
                return size.width();
        }
        RHETORIC_FATAL("never");
    }
    
    Option<float> GetMeasureQueryMaxValueFor(const MeasureQuery & query,
                                             TableDimension dimension)
    {
        switch (dimension) {
            case TableDimension::Row:
                return query.max_height();
            case TableDimension::Column:
                return query.max_width();
        }
        RHETORIC_FATAL("never");
    }
}
