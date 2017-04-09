#pragma once

#include "./dependency.h"

namespace oliview {
    extern std::ostream * print_output_stream;

    void Print(const std::string & string, bool new_line = true);
}
