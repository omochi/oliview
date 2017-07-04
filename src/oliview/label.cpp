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
    
    void Label::Layout() {
        auto app = application();
        auto ctx = app->nvg_context();
        auto fm = app->font_manager();
        
        auto font = this->font();
        if (!font) {
            font = fm->default_font();
        }
        
        nvgFontFaceId(ctx, font->nvg_handle());
        nvgFontSize(ctx, font_size());
        nvgFillColor(ctx, Color(0.0f, 0.0f, 0.0f, 1.0f).ToNanoVG());
        
        TextDrawLayouter layouter;
        
        text_layout_ = layouter.Layout(ctx,
                                       lines_,
                                       Some(frame().size().width()));
    }

    void Label::Draw() {
        View::Draw();
        
        auto app = application();
        
        auto ctx = app->nvg_context();
        
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
}
