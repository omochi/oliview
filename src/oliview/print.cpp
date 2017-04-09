#include "./print.h"

namespace oliview {
    std::ostream * print_output_stream = &std::cout;

    void Print(const std::string & string, bool new_line) {
        auto s = print_output_stream;
        if (!s) {
            return;
        }

        *s << string;
        if (new_line) {
            *s << "\n";
        }
    }
}
