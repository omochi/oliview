#pragma once

#include "./dependency.h"

namespace oliview {
    class StringSlice {
    public:
        StringSlice();
        StringSlice(const Ptr<std::string> & base);
        StringSlice(const Ptr<std::string> & base,
                    size_t offset,
                    size_t length);
        
        RHETORIC_GETTER(Ptr<std::string>, base)
        RHETORIC_GETTER(size_t, offset)
        
        const char * c_str() const;        
        RHETORIC_GETTER(size_t, length)
        
        StringSlice Slice(size_t offset) const;
        StringSlice Slice(size_t offset, size_t length) const;
        
        std::string AsString() const;
    private:
        Ptr<std::string> base_;
        size_t offset_;
        size_t length_;
    };
}
