#include "./text_box.h"

#include "./application.h"

namespace oliview {    
    void TextBox::Init(const Ptr<Application> & application) {
        View::Init(application);
        
        auto fm = application->font_manager();
        
        set_font(fm->default_font());
        set_font_size(12.0f);
        set_font_color(Color(0, 0, 0, 1));
    }
    
    std::string TextBox::text() const {
        auto ls = std::vector<std::string>(lines_.cbegin(), lines_.cend());
        return Join(ls, "");
    }
    
    void TextBox::set_text(const std::string & value) {
        auto ls = SplitLines(value);
        lines_ = std::list<std::string>(ls.cbegin(), ls.cend());
        
        SetNeedsLayout();
    }
    
    Size TextBox::MeasureContent(NVGcontext * ctx, const MeasureQuery & query) const {
        auto text_layout = LayoutText(ctx, query.max_width());
        return Size(text_layout.whole_frame.size());
    }
    
    void TextBox::LayoutContent(NVGcontext * ctx) {
        text_layout_ = LayoutText(ctx, Some(frame().size().width()));
    }
    
    void TextBox::DrawContent(NVGcontext * ctx) {
        NVGSetFont(ctx, font());
        nvgFontSize(ctx, font_size());
        NVGSetFillColor(ctx, font_color());
        
        float ascender;
        nvgTextMetrics(ctx, &ascender, nullptr, nullptr);
        
        for (auto & draw : text_layout_.entries) {
            nvgText(ctx,
                    0,
                    draw.y + ascender,
                    draw.string,
                    draw.string + draw.length);
        }
    }
    
    TextDrawLayouter::Result TextBox::LayoutText(NVGcontext * ctx,
                                                 const Optional<float> & max_width) const
    {
        NVGSetFont(ctx, font());
        nvgFontSize(ctx, font_size());
        NVGSetFillColor(ctx, font_color());
        
        TextDrawLayouter layouter;
        
        return layouter.Layout(ctx,
                               lines_,
                               max_width);
    }
}
