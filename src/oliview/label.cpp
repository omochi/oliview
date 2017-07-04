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
        
        SetNeedsLayout();
    }
    
    Size Label::OnMeasure(NVGcontext * ctx, const MeasureQuery & query) const {
        auto text_layout = LayoutText(ctx, query.max_width());
        return Size(text_layout.whole_frame.size());
    }
    
    void Label::OnLayout(NVGcontext * ctx) {
        text_layout_ = LayoutText(ctx, Some(frame().size().width()));
    }

    void Label::Draw(NVGcontext * ctx) {
        auto app = application();
        
        auto fm = app->font_manager();
        auto font = this->font();
        if (!font) {
            font = fm->default_font();
        }
        
        nvgFontFaceId(ctx, font->nvg_handle());
        nvgFontSize(ctx, font_size());
        nvgFillColor(ctx, Color(0.0f, 0.0f, 0.0f, 1.0f).ToNanoVG());
        
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
    
    TextDrawLayouter::Result Label::LayoutText(NVGcontext * ctx,
                                               const Optional<float> & max_width) const
    {
        auto app = application();
        auto fm = app->font_manager();
        
        auto font = this->font();
        if (!font) {
            font = fm->default_font();
        }
        
        nvgFontFaceId(ctx, font->nvg_handle());
        nvgFontSize(ctx, font_size());
        
        TextDrawLayouter layouter;
        
        return layouter.Layout(ctx,
                               lines_,
                               max_width);
    }
}
