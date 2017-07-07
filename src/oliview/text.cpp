#include "./text.h"

namespace oliview {
    Text::Position::Position():Position(0, 0){}
    
    Text::Position::Position(int line_index, int byte_offset):
    line_index_(line_index),
    byte_offset_(byte_offset)
    {}
    
    bool Text::Position::operator==(const Position & other) const {
        return line_index_ == other.line_index_ &&
        byte_offset_ == other.byte_offset_;
    }
    
    bool Text::Position::operator<(const Position & other) const {
        if (line_index_ == other.line_index_) {
            if (byte_offset_ < other.byte_offset_) {
                return true;
            }
        }
        if (line_index_ < other.line_index_) {
            return true;
        }
        return false;
    }
    
    std::string Text::Position::ToString() const {
        return Format("Text::Position(%d, %d)", line_index_, byte_offset_);
    }
    
    std::string Text::string() const {
        return JoinMap(lines_, "", [](auto x) { return *x; });
    }
    
    void Text::set_string(const std::string & value) {
        std::vector<Ptr<std::string>> lines;
        SplitLinesIterate(value, [&](auto x) {
            lines.push_back(New<std::string>(x));
        });
        set_lines(lines);
    }
    
    std::vector<Ptr<std::string>> Text::lines() const {
        return lines_;
    }
    
    void Text::set_lines(const std::vector<Ptr<std::string>> & value) {
        lines_ = value;
    }
    
    int Text::line_num() const {
        return (int)lines_.size();
    }
    
    Ptr<std::string> Text::GetLineAt(int index) const {
        return lines_[index];
    }
    
    void Text::SetLineAt(int index, const Ptr<std::string> & value) {
        lines_[index] = value;
    }
    
    StringSlice Text::GetCharAt(const Position & position) const {
        auto acc = AccessCharAt(position);
        return StringSlice(acc.string, acc.offset, acc.length);
    }
    
    void Text::SetCharAt(const Position & position, const std::string & chr) {
        auto acc = AccessCharAt(position);
        acc.string->replace(acc.offset, acc.length, chr);
    }
    
    Text::Position Text::begin_position() const {
        return SkipBodyBytes(Position(0, 0));
    }
    
    Text::Position Text::end_position() const {
        return Position((int)lines_.size(), 0);
    }
    
    Text::Position Text::AdvancePosition(const Position & pos_) const {
        Position pos = pos_;
        
        while (true) {
            if (pos == end_position()) {
                return pos;
            }

            auto acc = AccessCharAt(pos);
            switch (acc.kind.tag()) {
                case Utf8ByteKind::HeadTag: {
                    int len = acc.kind.AsHead().length;
                    if (acc.offset + len >= (int)acc.string->size()) {
                        return Position(pos.line_index() + 1, 0);
                    } else {
                        return Position(pos.line_index(), acc.offset + len);
                    }
                }
                case Utf8ByteKind::BodyTag: {
                    return SkipBodyBytes(pos);
                }
            }
        }
    }
    
    Text::StringAccess::StringAccess(const Ptr<std::string> & string,
                                     int offset,
                                     int length,
                                     Utf8ByteKind kind):
    string(string),
    offset(offset),
    length(length),
    kind(kind)
    {}
    
    Text::StringAccess Text::AccessCharAt(const Position & position) const {
        Ptr<std::string> str = lines_[position.line_index()];
        int start = position.byte_offset();
        char c = (*str)[start];
        auto kind = GetUtf8ByteKind((uint8_t)c);
        switch (kind.tag()) {
            case Utf8ByteKind::HeadTag: {
                int end = std::min(start + kind.AsHead().length, (int)str->size());
                return StringAccess(str,
                                    start,
                                    end - start,
                                    kind);
            }
                
            case Utf8ByteKind::BodyTag:
                return StringAccess(str,
                                    start,
                                    0,
                                    kind);
        }
        RHETORIC_FATAL("never");
    }
    
    Text::Position Text::SkipBodyBytes(const Position & pos_) const {
        Position pos = pos_;
        
        while (true) {
            if (pos == end_position()) {
                return pos;
            }
            
            auto acc = AccessCharAt(pos);
            if (acc.kind.tag() == Utf8ByteKind::HeadTag) {
                return pos;
            }
            
            pos = AdvancePositionByte(pos);
        }
    }
    
    Text::Position Text::AdvancePositionByte(const Position & pos_) const {
        auto pos = pos_;
        
        if (pos.line_index() >= (int)lines_.size()) {
            return pos;
        }
        
        auto * line = lines_[pos.line_index()].get();
        if (pos.byte_offset() + 1 >= (int)line->size()) {
            return Position(pos.line_index() + 1, 0);
        }
        
        pos.set_byte_offset(pos.byte_offset() + 1);
        
        return pos;
    }
}
