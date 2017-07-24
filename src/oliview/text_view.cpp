#include "./text_view.h"

#include "./application.h"

namespace oliview {
    void TextView::Init(const Ptr<Application> & application) {
        View::Init(application);
        
        text_layouter_ = New<TextDrawLayouter>();
        text_draw_info_ = nullptr;
        
        auto fm = application->font_manager();
        
        set_focusable(true);
        set_font(fm->default_font());
        set_font_size(12.0f);
        set_font_color(Color(0.0f, 0.0f, 0.0f, 1.0f));
        set_text(New<Text>());
        set_editable(true);
    }
    
    bool TextView::editable() const {
        return editable_;
    }
    
    void TextView::set_editable(bool value) {
        editable_ = value;
        
        // TODO
    }
    
    Ptr<const Text> TextView::text() const {
        return text_;
    }
    
    void TextView::set_text(const Ptr<const Text> & value) {
        text_ = New<Text>(*value);
        text_draw_info_ = nullptr;
        set_cursor_index(text_->begin_index());
        SetNeedsLayout();
    }
    
    void TextView::InsertTextAt(const Text::Index & index,
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
    
    void TextView::DeleteTextAt(const Text::Index & begin,
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
    
    Ptr<Font> TextView::font() const {
        return text_layouter_->font();
    }
    
    void TextView::set_font(const Ptr<Font> & value) {
        text_layouter_->set_font(value);
        SetNeedsLayout();
    }
    
    float TextView::font_size() const {
        return text_layouter_->font_size();
    }
    
    void TextView::set_font_size(float value) {
        text_layouter_->set_font_size(value);
        SetNeedsLayout();
    }
    
    Color TextView::font_color() const {
        return font_color_;
    }
    
    void TextView::set_font_color(const Color & value) {
        font_color_ = value;
    }
    
    Text::Index TextView::cursor_index() const {
        return cursor_index_;
    }
    
    void TextView::set_cursor_index(const Text::Index & value) {
        Print(Format("cursor index: %s", value.ToString().c_str()));
        RHETORIC_ASSERT(text_->CheckIndex(value));
        cursor_index_ = value;
        cursor_blink_time_ = 0.0f;
    }
    
    bool TextView::MoveCursorLeft() {
        if (!text_draw_info_) {
            return false;
        }
        
        auto index = text_->BackIndex(cursor_index_);
        if (index == cursor_index_) {
            return false;
        }
        set_cursor_index(index);
        cursor_x_ = GetCursorRect().origin().x();
        return true;
    }
    
    bool TextView::MoveCursorRight() {
        if (!text_draw_info_) {
            return false;
        }
        
        auto index = text_->AdvanceIndex(cursor_index_);
        if (index == cursor_index_) {
            return false;
        }
        set_cursor_index(index);
        cursor_x_ = GetCursorRect().origin().x();
        return true;
    }
    
    bool TextView::MoveCursorUp() {
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
    
    bool TextView::MoveCursorDown() {
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
    
    Size TextView::MeasureOwnContent(NVGcontext * ctx, const MeasureQuery & query) const {
        auto layout = text_layouter_->Layout(ctx,
                                             text_,
                                             query.max_width());
        return layout->frame().size();
    }
    
    void TextView::LayoutOwnContent(NVGcontext * ctx) {
        text_draw_info_ = text_layouter_->Layout(ctx,
                                                 text_,
                                                 Some(bounds().size().width()));
        float top = text_draw_info_->frame().origin().y();
        text_draw_info_->set_draw_offset(Vector2(0, -top));
    }
    
    void TextView::DrawOwnContent(NVGcontext * ctx) {
        RHETORIC_ASSERT(text_draw_info_ != nullptr);
        
        NVGSetFillColor(ctx, font_color());
        
        text_layouter_->Draw(ctx, text_, text_draw_info_);
        
        if (focused()) {
            if (cursor_blink_time_ <= 0.5f) {
                text_layouter_->DrawCursor(ctx, cursor_index(), text_draw_info_);
            }
        }
    }
    
    void TextView::OnUpdateAnimation(float delta_time) {
        // TODO: timer
        cursor_blink_time_ += delta_time;
        if (cursor_blink_time_ > 1.0f) {
            cursor_blink_time_ = 0.0f;
        }
    }
    
    bool TextView::OnMouseDownEvent(const MouseEvent & event) {
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
    
    bool TextView::OnKeyDownEvent(const KeyEvent & event) {
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
                if (!editable()) {
                    return false;
                }
                InsertTextAt(cursor_index(), New<Text>("\n"), nullptr);
                return true;
            case GLFW_KEY_BACKSPACE: {
                if (!editable()) {
                    return false;
                }
                auto end = cursor_index();
                auto begin = text_->BackIndex(end);
                if (begin == end) {
                    return false;
                }
                DeleteTextAt(begin, end);
                return true;
            }
            case GLFW_KEY_DELETE: {
                if (!editable()) {
                    return false;
                }
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
    }
    
    bool TextView::OnKeyRepeatEvent(const KeyEvent & event) {
        return OnKeyDownEvent(event);
    }
    
    void TextView::OnCharEvent(const CharEvent & event) {
        if (!editable()) {
            return;
        }
        Result<std::string> str_ret = EncodeUnicodeToUtf8(event.unicode());
        if (!str_ret) {
            return;
        }
        std::string str(*str_ret);
        InsertTextAt(cursor_index(), New<Text>(str), nullptr);
    }
    
    Text::Index TextView::GetTextIndexForLineIndexX(size_t line_index, float x) {
        RHETORIC_ASSERT(text_draw_info_ != nullptr);
        
        size_t char_index = text_draw_info_->GetCharIndexForX(line_index, x);
        TextDrawInfo::CharPositionIndex char_position_index(line_index, char_index);
        return text_draw_info_->GetTextIndexFor(char_position_index, text_);
    }

    Rect TextView::GetCursorRect() const {
        RHETORIC_ASSERT(text_draw_info_ != nullptr);
        return text_layouter_->GetCursorRect(cursor_index_, text_draw_info_);
    }
}
