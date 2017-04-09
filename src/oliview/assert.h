#pragma once

#include "./dependency.h"

#define OLIVIEW_ASSERT(condition) \
oliview::Assert(\
    condition, \
    #condition, \
    OLIVIEW_PRETTY_FUNCTION, \
    __FILE__, \
    __LINE__)

namespace oliview {
    void Assert(bool condition,
                const char * condition_str,
                const char * func,
                const char * file,
                int line);

    void Fatal(const std::string & message) OLIVIEW_NO_RETURN;
}
