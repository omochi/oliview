#pragma once

#include "./string_slice.h"

namespace oliview {
    class TextLineReader {
    public:
        TextLineReader(const Ptr<std::string> & string);
        
        Optional<StringSlice> Read();
    private:        
        Ptr<std::string> string_;
        
        size_t index_;
    };
}
