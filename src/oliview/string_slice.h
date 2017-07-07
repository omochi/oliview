#pragma once

#include "./dependency.h"

namespace oliview {
    class StringSlice {
    public:
        StringSlice();
        StringSlice(const Ptr<const std::string> & base);
        StringSlice(const Ptr<const std::string> & base,
                    int offset,
                    int length);
        
        RHETORIC_GETTER(Ptr<const std::string>, base)
        RHETORIC_GETTER(int, offset)
        
        const char * c_str() const;        
        RHETORIC_GETTER(int, length)
        
        StringSlice Slice(int offset) const;
        StringSlice Slice(int offset, int length) const;
        
        std::string AsString() const;
    private:
        Ptr<const std::string> base_;
        int offset_;
        int length_;
    };
}
