#include "./defer.h"

namespace oliview {
    Defer::Defer(const std::function<void()> proc):
    proc_(proc)
    {}

    Defer::~Defer(){
        if (proc_) {
            proc_();
        }
    }
}
