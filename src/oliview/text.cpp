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
    
    std::string Text::string() const {
        return JoinMap(lines_, "", [](auto x) { return *x; });
    }
    
    void Text::set_string(const std::string & value) {
        std::vector<Ptr<std::string>> lines;
        SplitLinesIterate(value, [&](auto x) {
            lines.push_back(New<std::string>(x));
        });
        set_line_ptrs(lines);
    }
    
    std::vector<Ptr<std::string>> Text::line_ptrs() const {
        return lines_;
    }
    
    void Text::set_line_ptrs(const std::vector<Ptr<std::string>> & value) {
        lines_ = value;
    }
    
    std::string Text::GetLineAt(int index) const {
        return *lines_[index];
    }
    
    void Text::SetLineAt(int index, const std::string & value) {
        lines_[index] = New<std::string>(value);
    }
    
    std::string Text::GetCharAt(const Position & position) const {
        auto acc = AccessCharAt(position);
        return acc.string->substr(acc.offset, acc.length);
    }
    
    void Text::SetCharAt(const Position & position, const std::string & chr) {
        auto acc = AccessCharAt(position);
        acc.string->replace(acc.offset, acc.length, chr);
    }
    
    Text::Position Text::begin_position() const {
        Position pos;
        
        while (true) {
            if (pos == end_position()) {
                break;
            }
            
            auto acc = AccessCharAt(pos);
            if (acc.kind.tag() == Utf8ByteKind::HeadTag) {
                break;
            }
            
            pos = AdvancePositionByte(pos);
        }
        
        return pos;
    }
    
    Text::Position Text::end_position() const {
        return Position((int)lines_.size(), 0);
    }
    
    Text::Position Text::AdvancePosition(const Position & pos_) const {
        Position pos = pos_;
        
        while (true) {
            if (pos == end_position()) {
                break;
            }

            auto acc = AccessCharAt(pos);
            switch (acc.kind.tag()) {
                case Utf8ByteKind::HeadTag: {
                    int len = acc.kind.AsHead().length;
                    if (acc.offset + len >= (int)acc.string->size()) {
                        pos = Position(pos.line_index() + 1, 0);
                    } else {
                        pos = Position(pos.line_index(), acc.offset + len);
                    }
                    break;
                }
                case Utf8ByteKind::BodyTag: {
                    pos = AdvancePositionByte(pos);
                    break;
                }
            }
        }
        
        return pos;
    }
    
    Text::StringAccess::StringAccess(std::string * string,
                                      int offset,
                                      int length,
                                      Utf8ByteKind kind):
    string(string),
    offset(offset),
    length(length),
    kind(kind)
    {}
    
    Text::StringAccess Text::AccessCharAt(const Position & position) const {
        std::string * str = lines_[position.line_index()].get();
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
