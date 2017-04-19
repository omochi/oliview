#include "./string_format.h"

#include "./assert.h"

namespace oliview {
    std::string Format(const char * format, ...) {
        va_list args;
        va_start(args, format);
        std::string ret = FormatV(format, args);
        va_end(args);
        return ret;
    }

    std::string FormatV(const char * format, va_list args) {
        va_list args2;
        va_copy(args2, args);

        std::string ret;
        std::vector<char> buf(4);

        int len = vsnprintf(buf.data(), buf.size(), format, args);
        if (len < 0) {
            Fatal("vsnprintf failed");
        }
        if (len < buf.size()) {
            ret = std::string(buf.data(), len);
        } else {
            buf.resize(len + 1);
            vsnprintf(buf.data(), buf.size(), format, args2);
            ret = std::string(buf.data(), len);
        }

        va_end(args2);
        return ret;
    }
}
