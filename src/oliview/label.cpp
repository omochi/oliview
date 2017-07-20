#include "./label.h"

#include "./application.h"

namespace oliview {
    void Label::Init(const Ptr<Application> & application) {
        Init(application, "");
    }
    
    void Label::Init(const Ptr<Application> & application,
                     const std::string & text)
    {
        View::Init(application);
        
        text_layouter_ = New<TextDrawLayouter>();
        text_ = New<Text>();
        
        auto fm = application->font_manager();
        
        set_font(fm->default_font());
        set_font_size(12.0f);
        set_font_color(Color(0, 0, 0, 1));
        set_text(text);
    }
    
    std::string Label::text() const {
        return text_->string();
    }
    
    void Label::set_text(const std::string & value) {
        text_ = New<Text>();
        text_->set_string(value);
        text_draw_info_ = nullptr;
        
        SetNeedsLayout();
    }
    
    Ptr<Font> Label::font() const {
        return text_layouter_->font();
    }

    void Label::set_font(const Ptr<Font> & value) {
        text_layouter_->set_font(value);
        
        SetNeedsLayout();
    }
    
    float Label::font_size() const {
        return text_layouter_->font_size();
    }
    
    void Label::set_font_size(float value) {
        text_layouter_->set_font_size(value);
        
        SetNeedsLayout();
    }
    
    Size Label::MeasureOwnContent(NVGcontext * ctx, const MeasureQuery & query) const {
        auto layout = text_layouter_->Layout(ctx,
                                             text_,
                                             query.max_width());
        return layout->frame().size();
    }
    
    void Label::LayoutOwnContent(NVGcontext * ctx) {
        text_draw_info_ = text_layouter_->Layout(ctx,
                                                 text_,
                                                 Some(frame().size().width()));
        float top = text_draw_info_->frame().origin().y();
        text_draw_info_->set_draw_offset(Vector2(0, -top));
    }

    void Label::DrawOwnContent(NVGcontext * ctx) {
        NVGSetFillColor(ctx, font_color());
        
        text_layouter_->Draw(ctx, text_, text_draw_info_);
    }

}
