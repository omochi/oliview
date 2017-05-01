#include "./generic_error.h"

namespace oliview {
    GenericError::GenericError(const std::string & message):
    message_(message)
    {}

    GenericError::~GenericError()
    {}

    std::string GenericError::message() const {
        return message_;
    }

    std::string GenericError::ToString() const {
        return Format("GenericError(%s)", message_.c_str());
    }

    Ref<GenericError> GenericError::Create(const char * format, ...) {
        va_list args;
        va_start(args, format);
        auto message = FormatV(format, args);
        va_end(args);

        Ref<GenericError> ret;
        ret.Attach(new GenericError(message));

        return ret;
    }
}
