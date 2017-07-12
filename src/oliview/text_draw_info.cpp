#include "./text_draw_info.h"

namespace oliview {
    TextDrawInfo::CharPosition::CharPosition(const Text::Index & text_index,
                                             float draw_x,
                                             float draw_left,
                                             float draw_right):
    text_index_(text_index),
    draw_x_(draw_x),
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
    
    size_t TextDrawInfo::LineEntry::char_position_num() const {
        return char_positions_.size();
    }
    
    Ptr<TextDrawInfo::CharPosition>
    TextDrawInfo::LineEntry::GetCharPositionAt(size_t index) const {
        return char_positions_[index];
    }
    
    StringSlice TextDrawInfo::LineEntry::GetLine(const Ptr<Text> & text) const {
        RHETORIC_ASSERT(char_positions_.size() > 0);
        
        if (char_positions_.size() == 0) {
            return StringSlice();
        }
        
        auto first_char = char_positions_.front()->GetChar(text);
        auto last_char = char_positions_.back()->GetChar(text);
        RHETORIC_ASSERT(first_char.base() == last_char.base());
        
        size_t len = ToUnsigned(last_char.c_str() + last_char.length() - first_char.c_str());
        return StringSlice(first_char.base(), first_char.offset(), len);
    }
    
    float TextDrawInfo::LineEntry::draw_width() const {
        if (char_positions_.size() == 0) {
            return 0;
        }
        return char_positions_.back()->draw_right();
    }
    
    TextDrawInfo::CharPositionIndex::CharPositionIndex(size_t line_index, size_t char_index):
    line_index(line_index),
    char_index(char_index)
    {}
    
    bool TextDrawInfo::CharPositionIndex::operator==(const CharPositionIndex & other) const {
        return line_index == other.line_index &&
        char_index == other.char_index;
    }
    
    size_t TextDrawInfo::line_num() const {
        return lines_.size();
    }
    
    Ptr<TextDrawInfo::LineEntry> TextDrawInfo::GetLineAt(size_t index) const {
        return lines_[index];
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
            if (line->text_index().line() != index.line()) {
                continue;
            }
            
            auto chars = line->char_positions();
            for (size_t char_index = 0; char_index < chars.size(); char_index++) {
                auto from_char = chars[char_index];
                if (from_char->text_index().byte() < index.byte()) {
                    continue;
                }
                
                return CharPositionIndex(line_index, char_index);
            }
            
            if ((line_index + 1 < lines_.size()) &&
                lines_[line_index]->wrapped_line()) {
                continue;
            }
            
            return CharPositionIndex(line_index, chars.size());
        }
        return end_index();
    }
    
    TextDrawInfo::CharPositionIndex
    TextDrawInfo::GetIndexFor(const Vector2 & position_) const
    {
        auto position = position_ - draw_offset();
        
        if (lines_.size() == 0) {
            return end_index();
        }
        
        CharPositionIndex ret;
        
        for (size_t line_index = 0; line_index < lines_.size(); line_index++) {
            if (line_index + 1 == lines_.size()) {
                if (position.y() < frame().end().y()) {
                    ret.line_index = line_index;
                    break;
                }
                
                {
                    ret.line_index = line_index;
                    auto line = lines_[line_index];
                    ret.char_index = line->char_position_num();
                    return ret;
                }
            }
            
            if (position.y() < GetLineTop(lines_[line_index + 1]->draw_y()))
            {
                ret.line_index = line_index;
                break;
            }
        }
        
        auto line = lines_[ret.line_index];
        for (size_t char_index = 0; char_index < line->char_position_num(); char_index++) {
            
            auto char_position = line->GetCharPositionAt(char_index);
            float char_center_x = (char_position->draw_left() + char_position->draw_right()) / 2.0f;
            if (position.x() < char_center_x) {
                ret.char_index = char_index;
                break;
            }
            
            if (char_index + 1 == line->char_position_num()) {
                ret.char_index = line->char_position_num();
                break;
            }
        }
        
        return ret;
    }
    
    Text::Index TextDrawInfo::GetTextIndexFor(const CharPositionIndex & position_index,
                                              const Ptr<Text> & text) const
    {
        if (position_index == end_index()) {
            return text->end_index();
        }
        
        auto line_entry = GetLineAt(position_index.line_index);
        
        if (line_entry->char_position_num() == 0) {
            return line_entry->text_index();
        }
        
        if (position_index.char_index < line_entry->char_position_num()) {
            auto char_entry = line_entry->GetCharPositionAt(position_index.char_index);
            return char_entry->text_index();
        }
        
        auto last_char = line_entry->GetCharPositionAt(line_entry->char_position_num() - 1);
        return text->AdvanceIndex(last_char->text_index());
    }
    
    Vector2 TextDrawInfo::GetDrawPointFor(const CharPositionIndex & position_index) const {
        if (position_index == end_index()) {
            return Vector2(0.0f, frame().end().y());
        }
        
        auto line_entry = GetLineAt(position_index.line_index);
        
        if (line_entry->char_position_num() == 0) {
            return Vector2(0, line_entry->draw_y());
        }
        
        if (position_index.char_index < line_entry->char_position_num()) {
            auto char_entry = line_entry->GetCharPositionAt(position_index.char_index);
            return Vector2(char_entry->draw_x(), line_entry->draw_y());
        }
        
        auto last_char = line_entry->GetCharPositionAt(line_entry->char_position_num() - 1);
        return Vector2(last_char->draw_right(), line_entry->draw_y());
    }
    
    float TextDrawInfo::GetLineTop(float y) const {
//        return y - font_ascent() - line_gap() / 2.0f;
        return y - font_ascent();
    }
    float TextDrawInfo::GetLineBottom(float y) const {
        return y - font_descent() + line_gap();
    }
}
