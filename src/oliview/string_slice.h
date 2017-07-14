#pragma once

#include "./dependency.h"

namespace oliview {
    template <typename STR> class StringSliceBase;
    
    using StringSlice = StringSliceBase<std::string>;
    using ConstStringSlice = StringSliceBase<const std::string>;
    
    template <typename STR>
    class StringSliceBase {
    public:
        StringSliceBase();
        StringSliceBase(const Ptr<STR> & base);
        StringSliceBase(const Ptr<STR> & base,
                        size_t offset,
                        size_t length);
        
        RHETORIC_GETTER(Ptr<STR>, base)
        RHETORIC_GETTER(size_t, offset)
        RHETORIC_GETTER(size_t, length)
        
        StringSliceBase<STR> Slice(size_t offset) const;
        StringSliceBase<STR> Slice(size_t offset, size_t length) const;
        
        std::remove_const_t<STR> AsString() const;
    private:
        Ptr<STR> base_;
        size_t offset_;
        size_t length_;
    };
}

#include "./string_slice_inline.h"

