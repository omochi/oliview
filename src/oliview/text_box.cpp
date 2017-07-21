#include "./text_box.h"

#include "./application.h"

namespace oliview {    
    void TextBox::Init(const Ptr<Application> & application) {
        ScrollView::Init(application);
        
        set_focusable(true);
        
        text_layouter_ = New<TextDrawLayouter>();
        text_ = New<Text>();
        cursor_index_ = text_->begin_index();
        cursor_x_ = 0;
        cursor_blink_time_ = 0.0f;
        
        auto fm = application->font_manager();
        
        set_font(fm->default_font());
        set_font_size(12.0f);
        set_font_color(Color(0, 0, 0, 1));
        
        text_view_ = OLIVIEW_INIT(TextBoxContentView, application, shared_from_this());
        text_view_->set_background_color(Color(0.9f, 0.9f, 1.0f, 1.0f));
        content_view()->AddChild(text_view_);
    }
    
    Ptr<const Text> TextBox::text() const {
        return text_;
    }
    
    void TextBox::set_text(const Ptr<const Text> & value) {
        text_ = New<Text>(*value);
        text_draw_info_ = nullptr;
        set_cursor_index(text_->begin_index());
        
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

    void TextBox::InsertTextAt(const Text::Index & index,
                               const Ptr<const Text> & text,
                               Text::Index * end_index_)
    {
        auto old_cursor_index = cursor_index_;
        
        Text::Index end_index;
        text_->InsertAt(index, text, &end_index);
        if (end_index_) {
            *end_index_ = end_index;
        }
        
        Text::Index cursor_index(old_cursor_index);
        
        if (index <= old_cursor_index) {
            if (index.line() == old_cursor_index.line()) {
                size_t byte_offset = old_cursor_index.byte() - index.byte();
                cursor_index = Text::Index(end_index.line(),
                                           end_index.byte() + byte_offset);
            } else {
                size_t line_offset = old_cursor_index.line() - index.line();
                cursor_index = Text::Index(end_index.line() + line_offset,
                                           old_cursor_index.byte());                
            }
        }
        
        if (old_cursor_index != cursor_index) {
            set_cursor_index(cursor_index);
        }
        
        SetNeedsLayout();
    }
    
    void TextBox::DeleteTextAt(const Text::Index & begin,
                               const Text::Index & end)
    {
        auto old_cursor_index = cursor_index_;

        text_->DeleteAt(begin, end);
        
        Text::Index cursor_index(old_cursor_index);

        if (begin <= old_cursor_index) {
            if (old_cursor_index <= end) {
                cursor_index = begin;
            } else {
                if (end.line() == old_cursor_index.line()) {
                    size_t byte_offset = old_cursor_index.byte() - end.byte();
                    cursor_index = Text::Index(begin.line(), begin.byte() + byte_offset);
                } else {
                    size_t line_offset = old_cursor_index.line() - end.line();
                    cursor_index = Text::Index(begin.line() + line_offset, old_cursor_index.byte());
                }
            }
        }
        
        if (old_cursor_index != cursor_index) {
            set_cursor_index(cursor_index);
        }
        
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
        if (index == cursor_index_) {
            return false;
        }
        set_cursor_index(index);
        cursor_x_ = GetCursorRect().origin().x();
        return true;
    }
    
    bool TextBox::MoveCursorRight() {
        auto index = text_->AdvanceIndex(cursor_index_);
        if (index == cursor_index_) {
            return false;
        }
        set_cursor_index(index);
        cursor_x_ = GetCursorRect().origin().x();
        return true;
    }

    bool TextBox::MoveCursorUp() {
        if (!text_draw_info_) {
            return false;
        }
        
        auto char_position = text_draw_info_->GetIndexFor(cursor_index_);
        if (char_position.line_index == 0) {
            return false;
        }
        auto new_index = GetTextIndexForLineIndexX(char_position.line_index - 1, cursor_x_);
        if (cursor_index_ == new_index) {
            return false;
        }
        set_cursor_index(new_index);
        return true;
    }
    
    bool TextBox::MoveCursorDown() {
        if (!text_draw_info_) {
            return false;
        }
        
        auto char_position = text_draw_info_->GetIndexFor(cursor_index_);
        if (char_position.line_index + 1 == text_draw_info_->line_num()) {
            return false;
        }
        auto new_index = GetTextIndexForLineIndexX(char_position.line_index + 1, cursor_x_);
        if (cursor_index_ == new_index) {
            return false;
        }
        set_cursor_index(new_index);
        return true;
    }
    
    Size TextBox::MeasureScrollContentView(NVGcontext * ctx,
                                           const Ptr<ScrollContentView> & view,
                                           const Size & visible_size) const
    {
        RHETORIC_UNUSED(view);
        auto layout = text_layouter_->Layout(ctx,
                                             text_,
                                             Some(visible_size.width()));
        Size text_size = layout->frame().size();
        Size size = text_size.GetMax(visible_size);
        return size;
    }
    
    void TextBox::LayoutScrollContentView(NVGcontext * ctx,
                                          const Ptr<ScrollContentView> & view)
    {
        RHETORIC_UNUSED(view);
        text_view_->set_frame(Rect(Vector2(),
                                   content_size()));
        
        text_draw_info_ = text_layouter_->Layout(ctx,
                                                 text_,
                                                 Some(content_size().width()));
        float top = text_draw_info_->frame().origin().y();
        text_draw_info_->set_draw_offset(Vector2(0, -top));
    }

    bool TextBox::OnKeyDownEvent(const KeyEvent & event) {
        switch (event.type()) {
            case KeyEventType::Down:
            case KeyEventType::Repeat:
                switch (event.key()) {
                    case GLFW_KEY_RIGHT:
                        return MoveCursorRight();
                    case GLFW_KEY_LEFT:
                        return MoveCursorLeft();
                    case GLFW_KEY_UP:
                        return MoveCursorUp();
                    case GLFW_KEY_DOWN:
                        return MoveCursorDown();
                    case GLFW_KEY_ENTER:
                        InsertTextAt(cursor_index(), New<Text>("\r\n"), nullptr);
                        return true;
                    case GLFW_KEY_BACKSPACE: {
                        auto end = cursor_index();
                        auto begin = text_->BackIndex(end);
                        if (begin == end) {
                            return false;
                        }
                        DeleteTextAt(begin, end);
                        return true;
                    }
                    case GLFW_KEY_DELETE: {
                        auto begin = cursor_index();
                        auto end = text_->AdvanceIndex(begin);
                        if (begin == end) {
                            return false;
                        }
                        DeleteTextAt(begin, end);
                        return true;
                    }
                    default:
                        return false;
                }
            default:
                return false;
        }
    }
    
    void TextBox::OnCharEvent(const CharEvent & event) {
        auto str_ret = EncodeUnicodeToUtf8(event.unicode());
        if (!str_ret) {
            return;
        }
        InsertTextAt(cursor_index(), New<Text>(*str_ret), nullptr);
    }
    
    void TextBox::OnUpdateAnimation(float delta_time) {
        cursor_blink_time_ += delta_time;
        if (cursor_blink_time_ > 1.0f) {
            cursor_blink_time_ = 0.0f;
        }
    }
    
    bool TextBox::_OnTextViewMouseDownEvent(const MouseEvent & event) {
        if (!text_draw_info_) {
            return false;
        }
        
        auto position_index = text_draw_info_->GetIndexFor(event.pos());
        auto text_index = text_draw_info_->GetTextIndexFor(position_index, text_);
        
        set_cursor_index(text_index);
        cursor_x_ = GetCursorRect().origin().x();
        
        this->Focus();
        
        return true;
    }
    
    void TextBox::_DrawTextViewContent(NVGcontext * ctx,
                                       const Ptr<TextBoxContentView> & view)
    {
        RHETORIC_UNUSED(view);
        RHETORIC_ASSERT(text_draw_info_ != nullptr);
        
        NVGSetFillColor(ctx, font_color());
        
        text_layouter_->Draw(ctx, text_, text_draw_info_);
        
        if (focused()) {
            if (cursor_blink_time_ <= 0.5f) {
                text_layouter_->DrawCursor(ctx, cursor_index(), text_draw_info_);
            }
        }
    }

    Text::Index TextBox::GetTextIndexForLineIndexX(size_t line_index, float x) {
        RHETORIC_ASSERT(text_draw_info_ != nullptr);
        
        size_t char_index = text_draw_info_->GetCharIndexForX(line_index, x);
        TextDrawInfo::CharPositionIndex char_position_index(line_index, char_index);
        return text_draw_info_->GetTextIndexFor(char_position_index, text_);
    }
    
    Rect TextBox::GetCursorRect() const {
        RHETORIC_ASSERT(text_draw_info_ != nullptr);
        return text_layouter_->GetCursorRect(cursor_index(), text_draw_info_);
    }
}
