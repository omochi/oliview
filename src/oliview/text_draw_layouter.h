#pragma once

#include "./rect.h"
#include "./size.h"

namespace oliview {
    class TextDrawLayouter {
    public:
        struct DrawEntry {
            const char * string;
            int length;
            
            float y;
            float draw_width;
            
            DrawEntry();
        };
        
        struct Result {
            std::vector<DrawEntry> entries;
            Rect whole_frame;
        };
        
        Result Layout(NVGcontext * ctx,
                      const std::list<std::string> & lines,
                      const Optional<float> & width_limit);
    private:
        struct SingleLineResult {
            int length;
            float draw_width;
            
            SingleLineResult();
        };
        
        std::vector<DrawEntry> LayoutLine(NVGcontext * ctx,
                                          const std::string & line,
                                          float draw_y,
                                          float * next_draw_y);
        
        SingleLineResult LayoutLineSingle(NVGcontext * ctx,
                                          const char * string_start,
                                          int string_length);
        
        float line_height_;
        Optional<float> width_limit_;
    };
}
