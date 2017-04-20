#pragma once

#include "./dependency.h"
#include "./error.h"
#include "./assert.h"
#include "./string_format.h"
#include "./ref.h"

namespace oliview {
    class PosixError : public Error {
    public:
        PosixError(int code, const std::string & description);

        virtual ~PosixError() {}

        virtual std::string ToString() const override;

        static Ref<PosixError> Create(int code, const char * format, ...) OLIVIEW_PRINTF_LIKE(2, 3);
    private:
        int code_;
        std::string description_;
    };

    std::string GetPosixErrorString(int code);
}

