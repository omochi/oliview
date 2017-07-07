#include "./text_box.h"

#include "./application.h"

namespace oliview {    
    void TextBox::Init(const Ptr<Application> & application) {
        View::Init(application);
        
        text_layouter_ = New<TextDrawLayouter>();
        text_ = New<Text>();
        
        auto fm = application->font_manager();
        
        set_font(fm->default_font());
        set_font_size(12.0f);
        set_font_color(Color(0, 0, 0, 1));
    }
    
    std::string TextBox::text() const {
        return text_->string();
    }
    
    void TextBox::set_text(const std::string & value) {
        text_->set_string(value);
        
        SetNeedsLayout();
    }
    
    Ptr<Font> TextBox::font() const {
        return text_layouter_->font();
    }
    
    void TextBox::set_font(const Ptr<Font> & value) {
        text_layouter_->set_font(value);
        
        SetNeedsLayout();
    }
    
    float TextBox::font_size() const {
        return text_layouter_->font_size();
    }
    
    void TextBox::set_font_size(float value) {
        text_layouter_->set_font_size(value);
        
        SetNeedsLayout();
    }

    Size TextBox::MeasureContent(NVGcontext * ctx, const MeasureQuery & query) const {
        auto layout = text_layouter_->Layout(ctx,
                                             text_,
                                             query.max_width());
        return text_layouter_->GetResultSize(ctx, layout);
    }
    
    void TextBox::LayoutContent(NVGcontext * ctx) {
        text_layout_result_ = text_layouter_->Layout(ctx,
                                                     text_,
                                                     Some(frame().size().width()));
    }
    
    void TextBox::DrawContent(NVGcontext * ctx) {
        NVGSetFillColor(ctx, font_color());
        
        text_layouter_->DrawResult(ctx, text_layout_result_);
    }
}
