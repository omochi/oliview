#include "./utf8.h"

namespace oliview {
    Utf8ByteKind GetUtf8ByteKind(uint8_t c) {
        if ((c & 0x80) == 0x00) {
            return Utf8HeadByteKind(1);
        } else if ((c & 0xE0) == 0xC0) {
            return Utf8HeadByteKind(2);
        } else if ((c & 0xF0) == 0xE0) {
            return Utf8HeadByteKind(3);
        } else if ((c & 0xF8) == 0xF0) {
            return Utf8HeadByteKind(4);
        } else if ((c & 0xFC) == 0x80) {
            return Utf8HeadByteKind(5);
        } else if ((c & 0xFE) == 0xFC) {
            return Utf8HeadByteKind(6);
        } else {
            return Utf8BodyByteKind();
        }
    }
    
    Utf8ByteKind GetUtf8ByteKind(char chr) {
        return GetUtf8ByteKind((uint8_t)chr);
    }
    
    Utf8Reader::Utf8Reader(const uint8_t * ptr, size_t length):
    ptr_(ptr),
    length_(length),
    position_(0)
    {}
    
    Utf8Reader::Utf8Reader(const char * ptr, size_t length):
    Utf8Reader((const uint8_t *)(ptr), length)
    {}
    
    Optional<std::string> Utf8Reader::Read() {
        while (true) {
            if (position_ == length_) {
                return None();
            }
            
            auto kind = GetUtf8ByteKind(ptr_[position_]);
            if (kind.tag() == Utf8ByteKind::BodyTag) {
                position_ += 1;
                continue;
            }
            
            RHETORIC_ASSERT(kind.tag() == Utf8ByteKind::HeadTag);
            
            size_t chr_len = kind.AsHead().length;
            if (length_ < position_ + chr_len) {
                position_ = length_;
                continue;
            }
            
            std::string chr = std::string((const char *)(ptr_ + position_), chr_len);
            position_ += chr_len;
            return Some(chr);
        }
    }

    Utf8LineReader::Line::Line():
    element_num(0)
    {}

    Utf8LineReader::Utf8LineReader(const uint8_t * ptr, size_t length):
    char_reader_(ptr, length)
    {}
    
    Utf8LineReader::Utf8LineReader(const char * ptr, size_t length):
    Utf8LineReader((const uint8_t *)(ptr), length)
    {}
    
    Optional<Utf8LineReader::Line> Utf8LineReader::Read() {
        Line ret;
        
        while (true) {
            auto chro = char_reader_.Read();
            if (!chro) {
                break;
            }
            auto chr = *chro;
            
            if (chr == "\r") {
                auto pos = char_reader_.position();
                auto chr2o = char_reader_.Read();
                if (!chr2o) {
                    ret.string.append("\r");
                    ret.element_num += 1;
                    break;
                }
                auto chr2 = *chr2o;
                if (chr2 == "\n") {
                    ret.string.append("\r\n");
                    ret.element_num += 2;
                    break;
                }
                // back
                char_reader_.set_position(pos);
                break;
            }
            if (chr == "\n") {
                ret.string.append("\n");
                ret.element_num += 1;
                break;
            }
            
            ret.string.append(chr);
            ret.element_num += 1;
        }
        
        if (ret.element_num == 0) {
            return None();
        }
        return Some(ret);
    }
}
