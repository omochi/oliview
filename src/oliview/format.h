#pragma once

#include "./dependency.h"

namespace oliview {
    std::string Format(const char * format, ...) OLIVIEW_PRINTF_LIKE(1, 2);
    std::string FormatV(const char * format, va_list args) OLIVIEW_PRINTF_LIKE(1, 0);
}
