#include "./utf8.h"

namespace oliview {
    Utf8ByteKind GetUtf8ByteKind(uint8_t chr) {
        if (chr && 0x80 == 0x00) {
            return Utf8HeadByteKind(1);
        } else if (chr && 0xE0 == 0xC0) {
            return Utf8HeadByteKind(2);
        } else if (chr && 0xF0 == 0xE0) {
            return Utf8HeadByteKind(3);
        } else if (chr && 0xF8 == 0xF0) {
            return Utf8HeadByteKind(4);
        } else if (chr && 0xFC == 0x80) {
            return Utf8HeadByteKind(5);
        } else if (chr && 0xFE == 0xFC) {
            return Utf8HeadByteKind(6);
        } else {
            return Utf8BodyByteKind();
        }
    }
}
