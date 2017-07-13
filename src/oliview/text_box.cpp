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

    bool TextBox::MoveCursorLeft() {
        auto index = text_->BackIndex(cursor_index_);
        if (index.line() != cursor_index_.line()) {
            return false;
        }
        if (index == cursor_index_) {
            return false;
        }
        set_cursor_index(index);
        return true;
    }
    
    bool TextBox::MoveCursorRight() {
        auto index = text_->AdvanceIndex(cursor_index_);
        if (index.line() != cursor_index_.line()) {
            return false;
        }
        if (index == cursor_index_) {
            return false;
        }
        set_cursor_index(index);
        return true;
    }

    bool TextBox::MoveCursorUp() {
        if (!text_draw_info_) {
            return false;
        }
        
        auto rect = text_layouter_->GetCursorRect(cursor_index_, text_draw_info_);
        Print(Format("%f, %f, %f, %f", rect.origin().x(), rect.origin().y(),
                     rect.size().width(), rect.size().height()));
        
        return true;
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
        
        if (focused()) {
            if (cursor_blink_time_ <= 0.5f) {
                text_layouter_->DrawCursor(ctx, cursor_index(), text_draw_info_);
            }
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
        
        Focus();
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
        switch (event.type()) {
            case KeyEventType::Down:
            case KeyEventType::Repeat: {
                if (event.key() == GLFW_KEY_RIGHT) {
                    return MoveCursorRight();
                } else if (event.key() == GLFW_KEY_LEFT) {
                    return MoveCursorLeft();
                } else if (event.key() == GLFW_KEY_UP) {
                    return MoveCursorUp();
                }
                
                break;
            }
            default:
                break;
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
