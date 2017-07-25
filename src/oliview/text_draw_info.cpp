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
    
    void TextDrawInfo::CharPosition::offset_draw_x(float offset) {
        draw_x_ += offset;
        draw_left_ += offset;
        draw_right_ += offset;
    }
    
    std::string TextDrawInfo::CharPosition::GetChar(const Ptr<Text> & text) const {
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
    
    std::string TextDrawInfo::LineEntry::GetLine(const Ptr<Text> & text) const {
        RHETORIC_ASSERT(char_positions_.size() > 0);
        
        if (char_positions_.size() == 0) {
            return "";
        }
        
        auto begin_index = char_positions_.front()->text_index();
        auto end_index = char_positions_.back()->text_index();
        RHETORIC_ASSERT(begin_index.line() == end_index.line());
        auto end_char = text->GetCharAt(end_index);
        
        size_t len = end_index.byte() + end_char.length() - begin_index.byte();
        return text->GetLineAt(begin_index.line())->substr(begin_index.byte(), len);
    }
    
    float TextDrawInfo::LineEntry::draw_x() const {
        if (char_positions_.size() == 0) {
            return 0;
        }
        return char_positions_.front()->draw_x();
    }
    
    float TextDrawInfo::LineEntry::draw_width() const {
        if (char_positions_.size() == 0) {
            return 0;
        }
        return char_positions_.back()->draw_right();
    }
    
    void TextDrawInfo::LineEntry::offset_draw_x(float offset) {
        for (auto & chr : char_positions_) {
            chr->offset_draw_x(offset);
        }
    }
    
    TextDrawInfo::CharPositionIndex::CharPositionIndex():CharPositionIndex(0, 0)
    {}
    
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
        if (lines_.size() == 0) {
            return CharPositionIndex(0, 0);
        }
        
        auto line_index = line_num() - 1;
        return CharPositionIndex(line_index, GetLineAt(line_index)->char_position_num());
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
            
            if (line->wrapped_line() && (line_index + 1 < lines_.size()))
            {
                continue;
//                auto next_line = lines_[line_index + 1];
//                if (next_line->char_position_num() > 0) {
//                    auto next_head_char = next_line->GetCharPositionAt(0);
//                    if (next_head_char->text_index().byte() < index.byte()) {
//                        continue;
//                    }
//                }
            }
            
            return CharPositionIndex(line_index, chars.size());
        }
        return end_index();
    }
    
    TextDrawInfo::CharPositionIndex
    TextDrawInfo::GetIndexFor(const Vector2 & position) const
    {
        auto line_ret = GetLineIndexForY(position.y());
        if (line_ret.under) {
            return begin_index();
        }
        if (line_ret.over) {
            return end_index();
        }
        
        auto line_index = line_ret.index.value();
        auto char_index = GetCharIndexForX(line_index, position.x());
        
        return CharPositionIndex(line_index, char_index);
    }
    
    TextDrawInfo::GetLineIndexResult::GetLineIndexResult():
    under(false),
    over(false)
    {}
    
    TextDrawInfo::GetLineIndexResult TextDrawInfo::GetLineIndexForY(float y) const {
        GetLineIndexResult ret;
        
        y = y - draw_offset().y();
     
        for (size_t line_index = 0; line_index < lines_.size(); line_index++) {
            if (line_index == 0) {
                if (y < GetLineTop(lines_[line_index]->draw_y())) {
                    ret.under = true;
                    return ret;
                }
            }
            
            if (y < GetLineBottom(lines_[line_index]->draw_y())) {
                ret.index = Some(line_index);
                return ret;
            }
        }
        
        ret.over = true;
        return ret;
    }
    
    size_t TextDrawInfo::GetCharIndexForX(size_t line_index, float x) const {
        GetLineIndexResult ret;
        
        x = x - draw_offset().x();
        
        auto line = lines_[line_index];
        for (size_t char_index = 0; char_index < line->char_position_num(); char_index++) {
            auto char_position = line->GetCharPositionAt(char_index);
            
            float char_center_x = (char_position->draw_left() + char_position->draw_right()) / 2.0f;
            if (x < char_center_x) {
                return char_index;
            }
        }
        
        return line->char_position_num();
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
        if (line_num() == 0) {
            return Vector2(origin_x(), 0);
        }
        
        auto line_entry = GetLineAt(position_index.line_index);
        
        if (line_entry->char_position_num() == 0) {
            return Vector2(origin_x(), line_entry->draw_y());
        }
        
        if (position_index.char_index < line_entry->char_position_num()) {
            auto char_entry = line_entry->GetCharPositionAt(position_index.char_index);
            return Vector2(char_entry->draw_x(), line_entry->draw_y());
        }
        
        auto last_char = line_entry->GetCharPositionAt(line_entry->char_position_num() - 1);
        return Vector2(last_char->draw_right(), line_entry->draw_y());
    }
    
    float TextDrawInfo::GetLineTop(float y) const {
        return y - font_ascent();
    }
    float TextDrawInfo::GetLineBottom(float y) const {
        return y - font_descent() + line_gap();
    }
}
