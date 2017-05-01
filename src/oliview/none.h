#pragma once

namespace oliview {
    struct None {
        bool operator==(const None & other) const;
        bool operator!=(const None & other) const;
    };
}
