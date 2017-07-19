#include "./axis.h"

namespace oliview {
    size_t AxisToIndex(Axis axis) {
        switch (axis) {
            case Axis::X:
                return 0;
            case Axis::Y:
                return 1;
        }
        RHETORIC_FATAL("never");
    }
    
    Result<Axis> AxisFromIndex(size_t index) {
        switch (index) {
            case 0:
                return Ok(Axis::X);
            case 1:
                return Ok(Axis::Y);
        }
        return GenericError::Create("invalid index: %" PRIdS, index);
    }
}
