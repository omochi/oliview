#pragma once

#include "./dependency.h"

#define OLIVIEW_DEFER(proc) \
_OLIVIEW_DEFER(__COUNTER__, proc)

namespace oliview {
    class Defer {
    public:
        Defer(const std::function<void()> proc);
        ~Defer();
    private:
        std::function<void()> proc_;
    };
}

#define _OLIVIEW_DEFER(counter, proc) \
__OLIVIEW_DEFER(counter, proc)

#define __OLIVIEW_DEFER(counter, proc) \
auto __defer__ ## counter = oliview::Defer(proc);
