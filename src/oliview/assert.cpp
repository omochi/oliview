#include "./assert.h"

#include "./string_format.h"

#include "./ref.h"
#include "./error.h"

namespace oliview {
    void Assert(bool condition,
                const char * condition_str,
                const char * func,
                const char * file,
                int line)
    {
        if (condition) {
            return;
        }

        std::string str = Format("assertion failure: [%s], func=[%s], file=[%s], line=%d",
                                 condition_str,
                                 func,
                                 file,
                                 line);
        Fatal(str);
    }

    void Fatal(const std::string & message) {
        throw std::logic_error(message);
    }

    void Fatal(const Ref<Error> & error) {
        Fatal(error->ToString());
    }
}
