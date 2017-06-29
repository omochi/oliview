#include "./text_draw_layouter.h"

namespace oliview {
    TextDrawLayouter::DrawEntry::DrawEntry():
    string(nullptr),
    length(0),
    y(0),
    left(0),
    right(0)
    {}
    
    TextDrawLayouter::Result
    TextDrawLayouter::Layout(NVGcontext * ctx,
                             const std::list<std::string> & lines,
                             const Optional<float> & width_limit)
    {
        width_limit_ = width_limit;
        
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
            Rect draw_rect = Rect(Vector2(draw.left, draw.y),
                                  Size(draw.right - draw.left, line_height_));
            whole_frame = whole_frame.GetUnion(draw_rect);
        }
        
        result.whole_frame = whole_frame;
        
        return result;
    }
    
    TextDrawLayouter::SingleLineResult::SingleLineResult():
    length(0),
    draw_left(0),
    draw_right(0)
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
            draw_entry.left = single_result.draw_left;
            draw_entry.right = single_result.draw_right;
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
            if (width_limit_) {
                if (index > 0 &&
                    *width_limit_ < position.maxx)
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
            result.draw_left = std::min(result.draw_left, position.minx);
            result.draw_right = std::max(result.draw_right, position.maxx);
            
            index += 1;
        }
    }
}
