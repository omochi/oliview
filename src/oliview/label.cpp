#include "./label.h"

#include "./application.h"

namespace oliview {
    Label::Label(const Ptr<Application> &application,
                 const std::string & text):
    View(application),
    font_size_(12.0f)
    {
        set_text(text);
    }
    
    std::string Label::text() const {
        auto ls = std::vector<std::string>(lines_.cbegin(), lines_.cend());
        return Join(ls, "");
    }
    
    void Label::set_text(const std::string & value) {
        auto ls = SplitLines(value);
        lines_ = std::list<std::string>(ls.cbegin(), ls.cend());
    }

    void Label::DrawContent(NVGcontext * ctx) {
        View::DrawContent(ctx);
        
        auto fm = application()->font_manager();
        auto font = this->font();
        if (!font) {
            font = fm->default_font();
        }
        
        nvgFontFaceId(ctx, font->nvg_handle());
        nvgFontSize(ctx, font_size());
        nvgFillColor(ctx, Color(0.0f, 0.0f, 0.0f, 1.0f).ToNanoVG());
        
        auto size = frame().size();
        
        float y = 0.0f;
        float line_height = 0.0f;
        float line_ascender = 0.0f;
        float line_descender = 0.0f;
        nvgTextMetrics(ctx, &line_ascender, &line_descender, &line_height);
        y += line_ascender;
                
        for (std::string line : lines_) {
            line = StripR(line, newline_chars());
            
            bool break_line_loop = false;
            
            const char * line_begin_ptr = line.c_str();
            const char * line_end_ptr = line_begin_ptr;
            while (true) {
                std::vector<NVGglyphPosition> positions(line.length());
                int num = nvgTextGlyphPositions(ctx,
                                                0,
                                                0,
                                                line_begin_ptr,
                                                nullptr,
                                                positions.data(),
                                                (int)positions.size());
                positions.resize(num);
                
                int position_index = 0;
                
                float draw_left = 0.0f;
                float draw_right = 0.0f;
                
                while (true) {
                    if (position_index >= (int)positions.size()) {
                        break_line_loop = true;
                        line_end_ptr = nullptr;
                        break; // line break
                    }
                    auto & position = positions[position_index];
                    if (position_index > 0 && size.x() < position.maxx) {
                        // wrap
                        line_end_ptr = position.str;
                        break;
                    }
                    
                    position_index += 1;
                    draw_left = std::min(draw_left, position.minx);
                    draw_right = std::max(draw_right, position.maxx);
                }
        
//                Print(Format("draw %f -- %f", draw_left, draw_right));
                nvgText(ctx, 0, y, line_begin_ptr, line_end_ptr);
                y += line_height;
                
                if (break_line_loop) {
                    break;
                }
                
                line_begin_ptr = line_end_ptr;
            }
        }
    }
}
