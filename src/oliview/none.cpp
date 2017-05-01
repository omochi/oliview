#include "./none.h"

namespace oliview {
    bool None::operator==(const None & other) const {
        return true;
    }

    bool None::operator!=(const None & other) const {
        return !(*this == other);
    }
}
