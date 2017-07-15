#pragma once

#include "./string_slice.h"

namespace oliview {
    class TextLineReader {
    public:
        TextLineReader(const Ptr<const std::string> & string);
        
        Option<std::string> Read();
    private:        
        Ptr<const std::string> string_;
        
        size_t index_;
    };
}
