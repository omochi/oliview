#pragma once

#include "./size.h"
#include "./string_slice.h"
#include "./text.h"

namespace oliview {
    class TextDrawInfo {
    public:
        class CharPosition {
        public:
            CharPosition(const Text::Position & text_position,
                         float draw_left,
                         float draw_right);
            
            RHETORIC_ACCESSOR(Text::Position, text_position)
            RHETORIC_ACCESSOR(float, draw_left)
            RHETORIC_ACCESSOR(float, draw_right)
            
            StringSlice GetChar(const Ptr<Text> & text) const;
        private:
            Text::Position text_position_;
            float draw_left_;
            float draw_right_;
        };
        
        class LineEntry {
        public:
            LineEntry(const Text::Position & text_position,
                      bool wrapped_line);
            
            RHETORIC_ACCESSOR(Text::Position, text_position)
            RHETORIC_ACCESSOR_TRIVIAL(bool, wrapped_line)
            RHETORIC_ACCESSOR(std::vector<Ptr<CharPosition>>, chars)
            int chars_num() const;
            
            StringSlice GetLine(const Ptr<Text> & text) const;
            
            float draw_width() const;
            
            RHETORIC_ACCESSOR_TRIVIAL(float, draw_y)
        private:
            Text::Position text_position_;
            bool wrapped_line_;
            std::vector<Ptr<CharPosition>> chars_;
            
            float draw_y_;
        };
        
        struct CharPositionIndex {
            int line_index;
            int char_index;
            
            CharPositionIndex(int line_index = 0, int char_index = 0);
            
            bool operator==(const CharPositionIndex & other) const;
            RHETORIC_EQUATABLE_DEFAULT(CharPositionIndex)
        };
        
        CharPositionIndex begin_index() const;
        CharPositionIndex end_index() const;
        
        CharPositionIndex GetIndexFor(const Text::Position & position) const;
        
        RHETORIC_ACCESSOR(std::vector<Ptr<LineEntry>>, lines)
        int line_num() const;
        RHETORIC_ACCESSOR(Size, size)
        
    private:
        std::vector<Ptr<LineEntry>> lines_;
        Size size_;
    };
    
}
