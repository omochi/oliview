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
        } else if ((c & 0xFC) == 0xF8) {
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
    
    Result<std::string> EncodeUnicodeToUtf8(uint32_t unicode) {
        uint8_t b[6];
        size_t len;
        if (unicode <= 0x007F) {
            // 0xxxxxxx
            len = 1;
            b[0] = unicode & 0x7F;
        } else if (unicode <= 0x07FF) {
            // 110yyyyx    10xxxxxx
            len = 2;
            b[0] = 0xC0 | ((unicode >> 6) & 0x1F);
            b[1] = 0x80 | (unicode & 0x3F);
        } else if (unicode <= 0xFFFF) {
            // 1110yyyy    10yxxxxx    10xxxxxx
            len = 3;
            b[0] = 0xE0 | ((unicode >> 12) & 0x0F);
            b[1] = 0x80 | ((unicode >> 6) & 0x3F);
            b[2] = 0x80 | (unicode & 0x3F);
        } else if (unicode <= 0x1FFFFF) {
            // 11110yyy    10yyxxxx    10xxxxxx    10xxxxxx
            len = 4;
            b[0] = 0xF0 | ((unicode >> 18) & 0x07);
            b[1] = 0x80 | ((unicode >> 12) & 0x3F);
            b[2] = 0x80 | ((unicode >> 6) & 0x3F);
            b[3] = 0x80 | (unicode & 0x3F);
        } else if (unicode <= 0x3FFFFFF) {
            // 111110yy    10yyyxxx    10xxxxxx    10xxxxxx    10xxxxxx
            len = 5;
            b[0] = 0xF8 | ((unicode >> 24) & 0x03);
            b[1] = 0x80 | ((unicode >> 18) & 0x3F);
            b[2] = 0x80 | ((unicode >> 12) & 0x3F);
            b[3] = 0x80 | ((unicode >> 6) & 0x3F);
            b[4] = 0x80 | (unicode & 0x3F);
        } else if (unicode <= 0x7FFFFFF) {
            // 1111110y    10yyyyxx    10xxxxxx    10xxxxxx    10xxxxxx    10xxxxxx
            len = 6;
            b[0] = 0xFC | ((unicode >> 30) & 0x01);
            b[1] = 0x80 | ((unicode >> 24) & 0x3F);
            b[2] = 0x80 | ((unicode >> 18) & 0x3F);
            b[3] = 0x80 | ((unicode >> 12) & 0x3F);
            b[4] = 0x80 | ((unicode >> 6) & 0x3F);
            b[5] = 0x80 | (unicode & 0x3F);
        } else {
            return GenericError::Create("invalid unicode for utf8: %08x", unicode);
        }
        return Ok(std::string((const char *)b, len));
    }
}
