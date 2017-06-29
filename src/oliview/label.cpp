#include "./label.h"

#include "./application.h"

namespace oliview {
    Label::Label(const Ptr<Application> &application,
                 const std::string & text):
    View(application),
    text_(text),
    font_size_(12.0f)
    {
    }

    void Label::DrawContent(NVGcontext * ctx) {
        View::DrawContent(ctx);
        
        auto fm = application()->font_manager();
        auto font = this->font();
        if (!font) {
            font = fm->default_font();
        }
        
        nvgFillColor(ctx, Color(0.0f, 0.0f, 0.0f, 1.0f).ToNanoVG());
        
        nvgFontFaceId(ctx, font->nvg_handle());
        nvgFontSize(ctx, font_size());
        nvgText(ctx, 0, 20, text_.c_str(), text_.c_str() + text_.size());
    }
}
