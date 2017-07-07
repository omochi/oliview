#pragma once

#include "./dependency.h"

namespace oliview {
    struct UnicodeScalarIndex {
        Range<int> index_range;
    };
    
    std::vector<UnicodeScalarIndex> GetUnicodeScalarIndices(const std::string & string);
    
    
}
