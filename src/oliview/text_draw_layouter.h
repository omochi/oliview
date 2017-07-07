#pragma once

#include "./nanovg_util.h"
#include "./rect.h"
#include "./size.h"
#include "./text.h"
#include "./utf8.h"

namespace oliview {
    class TextDrawLayouter {
    public:
        struct CharPosition {
            const char * chr;
            int index;
            int length;
            float left;
            float right;
            
            CharPosition(const char * chr,
                         int index,
                         int length,
                         float left,
                         float right);
        };
        class LineEntry {
        public:
            LineEntry(int original_line_index,
                      bool wrapped_line);
            
            RHETORIC_ACCESSOR_TRIVIAL(int, original_line_index)
            RHETORIC_ACCESSOR_TRIVIAL(bool, wrapped_line)
            RHETORIC_ACCESSOR(std::vector<Ptr<CharPosition>>, chars)
            int chars_num() const;
            
            const char * str() const;
            int length() const;
            
            float draw_width() const;
        private:
            int original_line_index_;
            bool wrapped_line_;
            std::vector<Ptr<CharPosition>> chars_;
        };
        
        struct Result {
            std::vector<LineEntry> lines;
        };
        
        RHETORIC_ACCESSOR(Ptr<Font>, font)
        RHETORIC_ACCESSOR_TRIVIAL(float, font_size)
        
        Result Layout(NVGcontext * ctx,
                      const Ptr<Text> & text,
                      const Optional<float> & max_width);
        
        Result LayoutLine(NVGcontext * ctx,
                          const char * line,
                          int line_length,
                          const Optional<float> & max_width);
        
        Size GetResultSize(NVGcontext * ctx,
                           const Result & result);
        
        void DrawResult(NVGcontext * ctx,
                        const Result & result);
    private:
        LineEntry LayoutSingleLine(NVGcontext * ctx,
                                   const char * line,
                                   int line_length);
        
        Ptr<Font> font_;
        float font_size_;
    };
}
