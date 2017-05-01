#pragma once

#include "./object.h"

namespace oliview {
    class Error : public Object<Error> {
    public:
        virtual ~Error() {}
        virtual std::string ToString() const = 0;
    };

}
