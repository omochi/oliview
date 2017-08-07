#pragma once

#include "./math.h"
#include "./rect.h"
#include "./size.h"
#include "./string_slice.h"
#include "./text.h"

namespace oliview {
    /*
     行数はTextと一致。
     末尾改行は描画に不要だしカーソル処理が面倒なのでなし。
     */
    
    class TextDrawInfo {
    public:
        class CharPosition {
        public:
            CharPosition(const Text::Index & text_index,
                         float draw_x,
                         float draw_left,
                         float draw_right);
            
            RHETORIC_ACCESSOR(Text::Index, text_index)
            RHETORIC_ACCESSOR(float, draw_x)
            RHETORIC_ACCESSOR(float, draw_left)
            RHETORIC_ACCESSOR(float, draw_right)
            
            void offset_draw_x(float offset);
            
            std::string GetChar(const Ptr<Text> & text) const;
        private:
            Text::Index text_index_;
            float draw_x_;
            float draw_left_;
            float draw_right_;
        };
        
        class LineEntry {
        public:
            LineEntry(const Text::Index & text_index,
                      bool wrapped_line);
            
            RHETORIC_ACCESSOR(Text::Index, text_index)
            RHETORIC_ACCESSOR_TRIVIAL(bool, wrapped_line)
            RHETORIC_ACCESSOR(std::vector<Ptr<CharPosition>>, char_positions)
            size_t char_position_num() const;
            Ptr<CharPosition> GetCharPositionAt(size_t index) const;
            RHETORIC_ACCESSOR(Option<std::string>, newline)
            std::string GetLine(const Ptr<Text> & text) const;
            
            float draw_x() const;
            float draw_width() const;
            RHETORIC_ACCESSOR_TRIVIAL(float, draw_y)
            
            void offset_draw_x(float offset);
        private:
            Text::Index text_index_;
            bool wrapped_line_;
            std::vector<Ptr<CharPosition>> char_positions_;
            Option<std::string> newline_;
            float draw_y_;
        };
        
        struct CharPositionIndex {
            size_t line_index;
            size_t char_index;
            
            CharPositionIndex();
            CharPositionIndex(size_t line_index, size_t char_index);
            
            bool operator==(const CharPositionIndex & other) const;
            RHETORIC_EQUATABLE_DEFAULT(CharPositionIndex)
        };
        
        struct FontMetrics {
            float ascent;
            float descent;
            float height;
            float line_height;
            float line_gap;
            
            FontMetrics();
        };

        RHETORIC_ACCESSOR(std::vector<Ptr<LineEntry>>, lines)
        size_t line_num() const;
        Ptr<LineEntry> GetLineAt(size_t index) const;
        
        
        CharPositionIndex begin_index() const;
        CharPositionIndex end_index() const;
        
        CharPositionIndex GetIndexFor(const Text::Index & index) const;
        CharPositionIndex GetIndexFor(const Vector2 & position) const;
        
        struct GetLineIndexResult {
            Option<size_t> index;
            bool under;
            bool over;
            GetLineIndexResult();
        };
        GetLineIndexResult GetLineIndexForY(float y) const;
        
        CharPositionIndex GetIndexFromX(size_t line_index, float x) const;
        bool IsWrappingPosition(const CharPositionIndex & index) const;
        
        Text::Index GetTextIndexFor(const CharPositionIndex & position_index,
                                    const Ptr<Text> & text) const;
        Vector2 GetDrawPointFor(const CharPositionIndex & position_index) const;
        
        RHETORIC_ACCESSOR(Rect, frame)
        RHETORIC_ACCESSOR(FontMetrics, font_metrics)
        
        float GetLineTop(float y) const;
        float GetLineBottom(float y) const;
        
        RHETORIC_ACCESSOR(float, origin_x)        
        RHETORIC_ACCESSOR(Vector2, draw_offset)
    private:
        std::vector<Ptr<LineEntry>> lines_;
        Rect frame_;
        
        FontMetrics font_metrics_;
        
        float origin_x_;
        
        Vector2 draw_offset_;

    };
    
}
