#pragma once

#include "./dependency.h"

namespace oliview {
    class CharEvent {
    public:
        RHETORIC_ACCESSOR_TRIVIAL(uint32_t, unicode)
    private:
        uint32_t unicode_;
    };
}
