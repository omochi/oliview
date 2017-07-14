namespace oliview {
    template <typename STR>
    StringSliceBase<STR>::StringSliceBase(): StringSliceBase(nullptr, 0, 0)
    {}
    
    template <typename STR>
    StringSliceBase<STR>::StringSliceBase(const Ptr<STR> & base):
    StringSliceBase(base, 0, base->size())
    {}
    
    template <typename STR>
    StringSliceBase<STR>::StringSliceBase(const Ptr<STR> & base,
                                          size_t offset,
                                          size_t length):
    base_(base),
    offset_(offset),
    length_(length)
    {}
    
    template <typename STR>
    StringSliceBase<STR> StringSliceBase<STR>::Slice(size_t offset) const {
        return Slice(offset, this->length() - offset);
    }
    
    template <typename STR>
    StringSliceBase<STR> StringSliceBase<STR>::Slice(size_t offset, size_t length) const {
        RHETORIC_ASSERT(offset >= 0);
        
        size_t start = this->offset() + offset;
        size_t end = std::min(start + length, this->offset() + this->length());
        
        return StringSliceBase<STR>(base_, start, end - start);
    }
    
    template <typename STR>
    std::remove_const_t<STR> StringSliceBase<STR>::AsString() const {
        if (!base_) {
            return STR();
        }
        return STR(base_->c_str() + offset(), length());
    }
}
