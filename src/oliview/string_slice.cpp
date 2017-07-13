#include "./string_slice.h"

namespace oliview {
    StringSlice::StringSlice(): StringSlice(nullptr, 0, 0)
    {}
    
    StringSlice::StringSlice(const Ptr<std::string> & base):
    StringSlice(base, 0, base->size())
    {}
    
    StringSlice::StringSlice(const Ptr<std::string> & base,
                             size_t offset,
                             size_t length):
    base_(base),
    offset_(offset),
    length_(length)
    {}
    
    const char * StringSlice::c_str() const {
        if (base_ == nullptr) {
            return nullptr;
        }
        return base_->c_str() + offset_;
    }
    
    StringSlice StringSlice::Slice(size_t offset) const {
        return Slice(offset, this->length() - offset);
    }
    
    StringSlice StringSlice::Slice(size_t offset, size_t length) const {
        RHETORIC_ASSERT(offset >= 0);
        
        size_t start = this->offset() + offset;
        size_t end = std::min(start + length, this->offset() + this->length());
        
        return StringSlice(base_, start, end - start);
    }
    
    std::string StringSlice::AsString() const {
        return std::string(c_str(), length());
    }
}
