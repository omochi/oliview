#pragma once

#include "./dependency.h"

namespace oliview {
    struct Utf8HeadByteKind {
        int length;
        
        Utf8HeadByteKind(int length):
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
    
    class Utf8Reader {
    public:
        Utf8Reader(const uint8_t * ptr, int length);
        Utf8Reader(const char * ptr, int length);
        
        RHETORIC_ACCESSOR_TRIVIAL(int, position)
        
        Optional<std::string> Read();
    private:
        const uint8_t * ptr_;
        int length_;
        int position_;
    };
    
    class Utf8LineReader {
    public:
        struct Line {
            std::string string;
            int element_num;
            
            Line();
        };
        
        Utf8LineReader(const uint8_t * ptr, int length);
        Utf8LineReader(const char * ptr, int length);
        
        Optional<Line> Read();
    private:
        Utf8Reader char_reader_;
    };
}
