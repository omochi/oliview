#include "./text_box.h"

#include "./application.h"

namespace oliview {    
    void TextBox::Init(const Ptr<Application> & application) {
        View::Init(application);
        
        set_focusable(true);
        
        text_layouter_ = New<TextDrawLayouter>();
        text_ = New<Text>();
        cursor_index_ = text_->begin_index();
        cursor_blink_time_ = 0.0f;
        
        auto fm = application->font_manager();
        
        set_font(fm->default_font());
        set_font_size(12.0f);
        set_font_color(Color(0, 0, 0, 1));
    }
    
    std::string TextBox::text() const {
        return text_->string();
    }
    
    void TextBox::set_text(const std::string & value) {
        text_ = New<Text>(value);
        text_draw_info_ = nullptr;
        cursor_index_ = text_->begin_index();
        
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
        Print(Format("cursor index: %s", value.ToString().c_str()));
        RHETORIC_ASSERT(text_->CheckIndex(value));
        cursor_index_ = value;
        cursor_blink_time_ = 0.0f;
    }

    Size TextBox::MeasureContent(NVGcontext * ctx, const MeasureQuery & query) const {
        auto layout = text_layouter_->Layout(ctx,
                                             text_,
                                             query.max_width());
        return layout->frame().size();
    }
    
    void TextBox::LayoutContent(NVGcontext * ctx) {
        text_draw_info_ = text_layouter_->Layout(ctx,
                                                 text_,
                                                 Some(frame().size().width()));
        float top = text_draw_info_->frame().origin().y();
        text_draw_info_->set_draw_offset(Vector2(0, -top));
    }
    
    void TextBox::DrawContent(NVGcontext * ctx) {
        RHETORIC_ASSERT(text_draw_info_ != nullptr);
        
        NVGSetFillColor(ctx, font_color());
        
        text_layouter_->Draw(ctx, text_, text_draw_info_);
        
        if (cursor_blink_time_ <= 0.5f) {
            text_layouter_->DrawCursor(ctx, cursor_index(), text_draw_info_);
        }
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
        if (!text_draw_info_) {
            return;
        }
        
        auto position_index = text_draw_info_->GetIndexFor(event.pos());
        auto text_index = text_draw_info_->GetTextIndexFor(position_index, text_);
        
        set_cursor_index(text_index);
    }
    
    void TextBox::OnMouseMoveEvent(const MouseEvent & event) {
        RHETORIC_UNUSED(event);
    }
    
    void TextBox::OnMouseUpEvent(const MouseEvent & event) {
        RHETORIC_UNUSED(event);
    }
    
    void TextBox::OnMouseCancelEvent() {
    }
    
    bool TextBox::OnKeyEvent(const KeyEvent & event) {
        if (event.type() == KeyEventType::Down) {
            Print(Format("TextBox %d", event.key()));
        }
        return false;
    }
    
    void TextBox::OnUpdateAnimation(float delta_time) {
        cursor_blink_time_ += delta_time;
        if (cursor_blink_time_ > 1.0f) {
            cursor_blink_time_ = 0.0f;
        }
    }
}
