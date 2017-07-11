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
        text_ = New<Text>();
        text_->set_string(value);
        text_draw_info_ = nullptr;
        
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
    
    void TextBox::set_cursor_index(const Text::Index & value) {
        RHETORIC_ASSERT(text_->CheckIndex(value));
        cursor_index_ = value;
    }

    Size TextBox::MeasureContent(NVGcontext * ctx, const MeasureQuery & query) const {
        auto layout = text_layouter_->Layout(ctx,
                                             text_,
                                             query.max_width());
        return layout->size();
    }
    
    void TextBox::LayoutContent(NVGcontext * ctx) {
        text_draw_info_ = text_layouter_->Layout(ctx,
                                                 text_,
                                                 Some(frame().size().width()));
    }
    
    void TextBox::DrawContent(NVGcontext * ctx) {
        NVGSetFillColor(ctx, font_color());
        
        text_layouter_->Draw(ctx, text_, text_draw_info_);
    }
    
    Ptr<View> TextBox::HitTest(const MouseEvent & event) {
        auto ret = View::HitTest(event);
        if (ret) {
            return ret;
        }
        if (IsPointInside(event.pos())) {
            return shared_from_this();
        }
        return nullptr;
    }
    
    void TextBox::OnMouseDownEvent(const MouseEvent & event) {
        Print(Format("down %f, %f", event.pos().x(), event.pos().y()));
    }
    
    void TextBox::OnMouseMoveEvent(const MouseEvent & event) {
        Print(Format("move %f, %f", event.pos().x(), event.pos().y()));
    }
    
    void TextBox::OnMouseUpEvent(const MouseEvent & event) {
        Print(Format("up %f, %f", event.pos().x(), event.pos().y()));
    }
    
    void TextBox::OnMouseCancelEvent() {
        
    }
}
