#include "./string_slice.h"

namespace oliview {
    StringSlice::StringSlice(): StringSlice(nullptr, 0, 0)
    {}
    
    StringSlice::StringSlice(const Ptr<const std::string> & base):
    StringSlice(base, 0, (int)base->size())
    {}
    
    StringSlice::StringSlice(const Ptr<const std::string> & base,
                             int offset,
                             int length):
    base_(base),
    offset_(offset),
    length_(length)
    {}
    
    const char * StringSlice::c_str() const {
        if (length() == 0) {
            return nullptr;
        }
        return base_->c_str() + offset_;
    }
    
    StringSlice StringSlice::Slice(int offset) const {
        return Slice(offset, this->length() - offset);
    }
    
    StringSlice StringSlice::Slice(int offset, int length) const {
        RHETORIC_ASSERT(offset >= 0);
        
        int start = this->offset() + offset;
        int end = std::min(start + length, this->offset() + this->length());
        
        return StringSlice(base_, start, end - start);
    }
    
    std::string StringSlice::AsString() const {
        return std::string(c_str(), length());
    }
}
