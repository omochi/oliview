#include "./posix_error.h"

namespace oliview {
    PosixError::PosixError(int code, const std::string & description):
    code_(code),
    description_(description)
    {}

    std::string PosixError::ToString() const {
        return Format("PosixError(%s(%d), %s)",
                      GetPosixErrorString(code_).c_str(),
                      code_,
                      description_.c_str());
    }

    Ref<PosixError> PosixError::Create(int code, const char * format, ...) {
        va_list args;
        va_start(args, format);
        auto desc = FormatV(format, args);
        va_end(args);
        return oliview::Create<PosixError>(code, desc);
    }

    std::string GetPosixErrorString(int code) {
        int size = strerror_r(code, nullptr, 0);
        std::vector<char> buf(size);
        size = strerror_r(code, buf.data(), buf.size());
        OLIVIEW_ASSERT(size == buf.size());
        return std::string(buf.data());
    }
}
