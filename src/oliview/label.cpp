#include "./label.h"

namespace oliview {
    Label::Label()
    {}

    Label::Label(const std::string & text) {
        set_text(text);
    }

    const std::string & Label::text() const {
        return text_;
    }

    void Label::set_text(const std::string & value) {
        text_ = value;
    }

    void Label::DrawContent(NVGcontext * ctx) {
        View::DrawContent(ctx);

//        nvgBeginPath(ctx);
//        nvgRect(ctx,
//                0,
//                0,
//                100,
//                20);
//        nvgFillColor(ctx, Color(0.0f, 1.0f, 0.0f, 1.0f).ToNanoVG());
//        nvgFill(ctx);

        nvgScissor(ctx, 0, 0, 30, 30);
        nvgFillColor(ctx, Color(0.0f, 0.0f, 0.0f, 1.0f).ToNanoVG());

        Font::default_font();
        nvgTextLineHeight(ctx, 10.0f);
        nvgText(ctx, 0, 20, text_.c_str(), text_.c_str() + text_.size());
    }
}
