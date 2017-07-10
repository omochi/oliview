#include "./text_draw_info.h"

namespace oliview {
    TextDrawInfo::CharPosition::CharPosition(const Text::Index & text_index,
                                             float draw_left,
                                             float draw_right):
    text_index_(text_index),
    draw_left_(draw_left),
    draw_right_(draw_right)
    {}
    
    StringSlice TextDrawInfo::CharPosition::GetChar(const Ptr<Text> & text) const {
        return text->GetCharAt(text_index_);
    }
    
    TextDrawInfo::LineEntry::LineEntry(const Text::Index & text_index,
                                       bool wrapped_line):
    text_index_(text_index),
    wrapped_line_(wrapped_line),
    draw_y_(0)
    {}
    
    size_t TextDrawInfo::LineEntry::chars_num() const {
        return chars_.size();
    }
    
    StringSlice TextDrawInfo::LineEntry::GetLine(const Ptr<Text> & text) const {
        if (chars_.size() == 0) {
            return StringSlice();
        }
        
        auto first_char = chars_.front()->GetChar(text);
        auto last_char = chars_.back()->GetChar(text);
        RHETORIC_ASSERT(first_char.base() == last_char.base());
        
        size_t len = ToUnsigned(last_char.c_str() + last_char.length() - first_char.c_str());
        return StringSlice(first_char.base(), first_char.offset(), len);
    }
    
    float TextDrawInfo::LineEntry::draw_width() const {
        if (chars_.size() == 0) {
            return 0;
        }
        return chars_.back()->draw_right();
    }
    
    TextDrawInfo::CharPositionIndex::CharPositionIndex(size_t line_index, size_t char_index):
    line_index(line_index),
    char_index(char_index)
    {}
    
    bool TextDrawInfo::CharPositionIndex::operator==(const CharPositionIndex & other) const {
        return line_index == other.line_index &&
        char_index == other.char_index;
    }
    
    TextDrawInfo::CharPositionIndex TextDrawInfo::begin_index() const {
        return CharPositionIndex(0, 0);
    }
    
    TextDrawInfo::CharPositionIndex TextDrawInfo::end_index() const {
        return CharPositionIndex(lines_.size(), 0);
    }
    
    TextDrawInfo::CharPositionIndex
    TextDrawInfo::GetIndexFor(const Text::Index & index) const
    {
        for (size_t line_index = 0; line_index < lines_.size(); line_index++) {
            auto line = lines_[line_index];
            if (line->text_index().line() < index.line()) {
                continue;
            }
            auto line_chars = line->chars();
            for (size_t char_index = 0; char_index < line_chars.size(); char_index++) {
                auto from_char = line_chars[char_index];
                if (from_char->text_index() < index) {
                    continue;
                }
                
                return CharPositionIndex(line_index, char_index);
            }
        }
        return end_index();
    }
    
    size_t TextDrawInfo::line_num() const {
        return lines_.size();
    }

}
