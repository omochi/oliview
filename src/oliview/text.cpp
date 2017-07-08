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
            if (acc.length == 0) {
                RHETORIC_ASSERT(!acc.kind.presented());
                return Position(pos.line_index() + 1, 0);
            }
            
            switch (acc.kind->tag()) {
                case Utf8ByteKind::HeadTag: {
                    int len = acc.kind->AsHead().length;
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
    
    Text::Position Text::FixPosition(const Position & pos) const {
        int line_index = pos.line_index();
        int offset = pos.byte_offset();
        if (line_index < 0) {
            line_index = 0;
            offset = 0;
        } else if (line_index >= line_num()) {
            line_index = line_num();
            offset = 0;
        } else {
            auto line = GetLineAt(line_index);
            offset = Clamp(offset, 0, (int)line->size());
        }
        return Position(line_index, offset);
    }
    
    void Text::Insert(const Text::Position & position,
                      const std::string & string,
                      Text::Position * result_position)
    {
        
    }
    
    Text::StringAccess::StringAccess(const Ptr<std::string> & string,
                                     int offset,
                                     int length,
                                     Optional<Utf8ByteKind> kind):
    string(string),
    offset(offset),
    length(length),
    kind(kind)
    {}
    
    Text::StringAccess Text::AccessCharAt(const Position & position) const {
        Ptr<std::string> str = GetLineAt(position.line_index());
        int offset = position.byte_offset();
        if (offset == (int)str->size()) {
            return StringAccess(str,
                                offset,
                                0,
                                None());
        }
        
        char c = (*str)[offset];
        auto kind = GetUtf8ByteKind((uint8_t)c);
        switch (kind.tag()) {
            case Utf8ByteKind::HeadTag: {
                int end = std::min(offset + kind.AsHead().length, (int)str->size());
                return StringAccess(str,
                                    offset,
                                    end - offset,
                                    Some(kind));
            }
                
            case Utf8ByteKind::BodyTag:
                return StringAccess(str,
                                    offset,
                                    0,
                                    Some(kind));
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
            if (acc.kind && acc.kind->tag() == Utf8ByteKind::HeadTag) {
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
