#include "./text_draw_layouter.h"

namespace oliview {
    TextDrawLayouter::DrawEntry::DrawEntry():
    string(nullptr),
    length(0),
    y(0),
    draw_width(0)
    {}
    
    TextDrawLayouter::Result
    TextDrawLayouter::Layout(NVGcontext * ctx,
                             const std::list<std::string> & lines,
                             const Optional<float> & max_width)
    {
        max_width_ = max_width;
        
        Result result;
        
        float draw_y = 0.0f;
        
        nvgTextMetrics(ctx, nullptr, nullptr, &line_height_);
        
        for (auto & line : lines) {
            auto draws = LayoutLine(ctx, line, draw_y, &draw_y);
            
            for (auto & draw : draws) {
                result.entries.push_back(draw);
            }
        }
        
        Rect whole_frame;
        
        for (auto & draw : result.entries) {
            Rect draw_rect = Rect(Vector2(0, draw.y),
                                  Size(draw.draw_width, line_height_));
            whole_frame = whole_frame.GetUnion(draw_rect);
        }
        
        result.whole_frame = whole_frame;
        
        return result;
    }
    
    TextDrawLayouter::SingleLineResult::SingleLineResult():
    length(0),
    draw_width(0)
    {}
    
    std::vector<TextDrawLayouter::DrawEntry>
    TextDrawLayouter::LayoutLine(NVGcontext * ctx,
                                 const std::string & line,
                                 float draw_y,
                                 float * next_draw_y)
    {
        std::vector<DrawEntry> result;
        
        int end_index;
        auto find_ret = Find(line, 0, newline_chars());
        if (find_ret) {
            end_index = find_ret->index;
        } else {
            end_index = (int)line.size();
        }
        
        const char * const line_end_ptr = line.c_str() + end_index;
        const char * next_begin_ptr = line.c_str();
        
        while (true) {
            SingleLineResult single_result = LayoutLineSingle(ctx,
                                                              next_begin_ptr,
                                                              (int)(line_end_ptr - next_begin_ptr));
            
            DrawEntry draw_entry;
            draw_entry.string = next_begin_ptr;
            draw_entry.length = single_result.length;
            draw_entry.y = draw_y;
            draw_entry.draw_width = single_result.draw_width;
            result.push_back(draw_entry);
            
            draw_y += line_height_;
            
            next_begin_ptr += single_result.length;
            
            if (line_end_ptr <= next_begin_ptr) {
                break;
            }
        }
        
        *next_draw_y = draw_y;
        
        return result;
    }
    
    TextDrawLayouter::SingleLineResult
    TextDrawLayouter::LayoutLineSingle(NVGcontext * ctx,
                                       const char * string_start,
                                       int string_length)
    {
        SingleLineResult result;
        
        std::vector<NVGglyphPosition> positions(string_length);
        int num = nvgTextGlyphPositions(ctx,
                                        0,
                                        0,
                                        string_start,
                                        string_start + string_length,
                                        positions.data(),
                                        (int)positions.size());
        positions.resize(num);
        
        int index = 0;
        
        while (true) {
            if (index >= (int)positions.size()) {
                //  string end
                return result;
            }
            
            auto & position = positions[index];
            if (max_width_) {
                if (index > 0 &&
                    *max_width_ < position.maxx)
                {
                    //  wrap
                    return result;
                }
            }
            
            const char * next_glyph_str;
            if (index + 1 < (int)positions.size()) {
                next_glyph_str = positions[index + 1].str;
            } else {
                next_glyph_str = string_start + string_length;
            }
            
            result.length = (int)(next_glyph_str - string_start);
            result.draw_width = std::max(result.draw_width, position.maxx);
            
            index += 1;
        }
    }
}
