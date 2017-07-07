#include "./text_draw_info.h"

namespace oliview {
    TextDrawInfo::CharPosition::CharPosition(const Text::Position & text_position,
                                             float draw_left,
                                             float draw_right):
    text_position_(text_position),
    draw_left_(draw_left),
    draw_right_(draw_right)
    {}
    
    StringSlice TextDrawInfo::CharPosition::GetChar(const Ptr<Text> & text) const {
        return text->GetCharAt(text_position_);
    }
    
    TextDrawInfo::LineEntry::LineEntry(const Text::Position & text_position,
                                       bool wrapped_line):
    text_position_(text_position),
    wrapped_line_(wrapped_line),
    draw_y_(0)
    {}
    
    int TextDrawInfo::LineEntry::chars_num() const {
        return (int)chars_.size();
    }
    
    StringSlice TextDrawInfo::LineEntry::GetLine(const Ptr<Text> & text) const {
        if (chars_.size() == 0) {
            return StringSlice();
        }
        
        auto first_char = chars_.front()->GetChar(text);
        auto last_char = chars_.back()->GetChar(text);
        RHETORIC_ASSERT(first_char.base() == last_char.base());
        
        int len = (int)(last_char.c_str() + last_char.length() - first_char.c_str());
        return StringSlice(first_char.base(), first_char.offset(), len);
    }
    
    float TextDrawInfo::LineEntry::draw_width() const {
        if (chars_.size() == 0) {
            return 0;
        }
        return chars_.back()->draw_right();
    }
    
    TextDrawInfo::CharPositionIndex::CharPositionIndex(int line_index, int char_index):
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
        return CharPositionIndex((int)lines_.size(), 0);
    }
    
    TextDrawInfo::CharPositionIndex
    TextDrawInfo::GetIndexFor(const Text::Position & position) const
    {
        for (int line_index = 0; line_index < (int)lines_.size(); line_index++) {
            auto line = lines_[line_index];
            if (line->text_position().line_index() < position.line_index()) {
                continue;
            }
            auto line_chars = line->chars();
            for (int char_index = 0; char_index < (int)line_chars.size(); char_index++) {
                auto from_char = line_chars[char_index];
                if (from_char->text_position() < position) {
                    continue;
                }
                
                return CharPositionIndex(line_index, char_index);
            }
        }
        return end_index();
    }
    
    int TextDrawInfo::line_num() const {
        return (int)lines_.size();
    }

}
