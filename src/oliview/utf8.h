#pragma once

#include "./dependency.h"

namespace oliview {
    struct Utf8HeadByteKind {
        size_t length;
        
        Utf8HeadByteKind(size_t length):
        length(length) {};
    };
    struct Utf8BodyByteKind {};
    
    class Utf8ByteKind {
    public:
        enum Tag : int8_t {
            HeadTag = 0,
            BodyTag = 1
        };
        
        Utf8ByteKind() = delete;
        Utf8ByteKind(const Utf8HeadByteKind & x): either_(EitherCase<Either2Tag::Case0>(x)) {}
        Utf8ByteKind(const Utf8BodyByteKind & x): either_(EitherCase<Either2Tag::Case1>(x)) {}
        
        Tag tag() const { return static_cast<Tag>(either_.tag()); }
        
        Utf8HeadByteKind AsHead() const { return either_.AsCase0(); }
        Utf8BodyByteKind AsBody() const { return either_.AsCase1(); }
    private:
        Either2<Utf8HeadByteKind, Utf8BodyByteKind> either_;
    };

    Utf8ByteKind GetUtf8ByteKind(uint8_t chr);
    Utf8ByteKind GetUtf8ByteKind(char chr);
    
    Result<std::string> EncodeUnicodeToUtf8(uint32_t unicode);
}
