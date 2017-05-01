#pragma once

#include "./error.h"
#include "./string_format.h"

namespace oliview {
    class GenericError : public Error {
    public:
        GenericError(const std::string & message);
        virtual ~GenericError();

        std::string message() const;

        virtual std::string ToString() const;

        static Ref<GenericError> Create(const char * format, ...) OLIVIEW_PRINTF_LIKE(1, 2);
    private:
        std::string message_;
    };
}
