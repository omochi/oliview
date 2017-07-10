#include "./text.h"

namespace oliview {
    Text::Position::Position():Position(0, 0){}
    
    Text::Position::Position(size_t line_index, size_t byte_offset):
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
        return Format("Text::Position(%" PRIdS ", %" PRIdS ")", line_index_, byte_offset_);
    }
    
    Text::Text() {}
    
    Text::Text(const std::string & value) {
        set_string(value);
    }
    
    std::string Text::string() const {
        auto ls = ArrayMap(lines_, [](auto x) { return *x; });
        return Join(ls, "");
    }
    
    void Text::set_string(const std::string & value) {
        std::vector<Ptr<std::string>> lines;
        
        auto reader = Utf8LineReader(value.c_str(), value.size());
        while (true) {
            auto line = reader.Read();
            if (!line) {
                break;
            }
            lines.push_back(New<std::string>(line->string));
        }
        
        set_lines(lines);
    }
    
    std::vector<Ptr<std::string>> Text::lines() const {
        return lines_;
    }
    
    void Text::set_lines(const std::vector<Ptr<std::string>> & value) {
        lines_ = value;
    }
    
    size_t Text::line_num() const {
        return lines_.size();
    }
    
    Ptr<std::string> Text::GetLineAt(size_t index) const {
        return lines_[index];
    }
    
    void Text::SetLineAt(size_t index, const Ptr<std::string> & value) {
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
        return Position(lines_.size(), 0);
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
                    size_t len = acc.kind->AsHead().length;
                    if (acc.offset + len >= acc.string->size()) {
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
    
    bool Text::CheckPosition(const Position & pos) const {
        if (pos.line_index() > lines_.size()) {
            return false;
        }
        if (pos.line_index() == lines_.size()) {
            if (pos.byte_offset() == 0) {
                return true;
            } else {
                return false;
            }
        }
        auto line = lines_[pos.line_index()];
        if (line->size() < pos.byte_offset()) {
            return false;
        }
        auto kind = GetUtf8ByteKind((*line)[pos.byte_offset()]);
        if (kind.tag() == Utf8ByteKind::HeadTag) {
            return true;
        }
        return false;
    }
    
    void Text::Insert(const Text::Position & position_,
                      const Ptr<Text> & text,
                      Text::Position * result_position)
    {
        auto position = position_;
        
        RHETORIC_ASSERT(CheckPosition(position));

        RHETORIC_UNUSED(result_position);
        
        if (position.line_index() == lines_.size()) {
            lines_.push_back(New<std::string>());
        }
        
        Ptr<std::string> dest_line = lines_[position.line_index()];
        if (position.byte_offset() == dest_line->size()) {
            if (CheckEndWith(*dest_line, dest_line->size(), newline_chars())) {
                position = Position(position.line_index() + 1, 0);
            }
        }
        
        for (auto insert_line : text->lines()) {
            if (position.line_index() == lines_.size()) {
                dest_line = New<std::string>();
                lines_.push_back(dest_line);
            } else {
                dest_line = lines_[position.line_index()];
            }
            
            dest_line->insert(position.byte_offset(), *insert_line);
            position = Position(position.line_index(), position.byte_offset() + insert_line->size());

            if (CheckEndWith(*insert_line, insert_line->size(), newline_chars())) {
                if (position.byte_offset() < dest_line->size()) {
                    auto newline_line = dest_line->substr(position.byte_offset(), dest_line->size() - position.byte_offset());
                    ArrayRemoveRange(dest_line.get(), MakeRange(position.byte_offset(), dest_line->size()));
                    lines_.insert(lines_.begin() + ToSigned(position.line_index()) + 1, New<std::string>(newline_line));
                }
                
                position = Position(position.line_index() + 1, 0);
            }
        }
    }
    
    Text::StringAccess::StringAccess(const Ptr<std::string> & string,
                                     size_t offset,
                                     size_t length,
                                     Optional<Utf8ByteKind> kind):
    string(string),
    offset(offset),
    length(length),
    kind(kind)
    {}
    
    Text::StringAccess Text::AccessCharAt(const Position & position) const {
        Ptr<std::string> str = GetLineAt(position.line_index());
        size_t offset = position.byte_offset();
        if (offset == str->size()) {
            return StringAccess(str,
                                offset,
                                0,
                                None());
        }
        
        char c = (*str)[offset];
        auto kind = GetUtf8ByteKind(c);
        switch (kind.tag()) {
            case Utf8ByteKind::HeadTag: {
                size_t end = std::min(offset + kind.AsHead().length, str->size());
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
        
        if (pos.line_index() >= lines_.size()) {
            return pos;
        }
        
        auto * line = lines_[pos.line_index()].get();
        if (pos.byte_offset() + 1 >= line->size()) {
            return Position(pos.line_index() + 1, 0);
        }
        
        pos.set_byte_offset(pos.byte_offset() + 1);
        
        return pos;
    }
}
