#include "./text_view_base.h"

#include "./application.h"

namespace oliview {
    void TextViewBase::Init(const Ptr<Application> & application) {
        View::Init(application);
        
        text_layouter_ = New<TextDrawLayouter>();
        text_draw_info_ = nullptr;
        
        auto fm = application->font_manager();
        
        _set_cursor_visible(false);
        set_focusable(true);
        set_font(fm->default_font());
        set_font_size(12.0f);
        set_font_color(Color(0.0f, 0.0f, 0.0f, 1.0f));
        set_text(New<Text>());
        set_editable(true);
    }
    
    bool TextViewBase::editable() const {
        return editable_;
    }
    
    void TextViewBase::set_editable(bool value) {
        editable_ = value;
    }
    
    Ptr<const Text> TextViewBase::text() const {
        return text_;
    }
    
    void TextViewBase::set_text(const Ptr<const Text> & value) {
        text_ = New<Text>(*value);
        text_draw_info_ = nullptr;
        set_cursor_index(text_->begin_index());
        SetNeedsLayout();
    }
    
    void TextViewBase::InsertTextAt(const Text::Index & index,
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
    
    void TextViewBase::DeleteTextAt(const Text::Index & begin,
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
    
    Ptr<Font> TextViewBase::font() const {
        return text_layouter_->font();
    }
    
    void TextViewBase::set_font(const Ptr<Font> & value) {
        text_layouter_->set_font(value);
        SetNeedsLayout();
    }
    
    float TextViewBase::font_size() const {
        return text_layouter_->font_size();
    }
    
    void TextViewBase::set_font_size(float value) {
        text_layouter_->set_font_size(value);
        SetNeedsLayout();
    }
    
    Color TextViewBase::font_color() const {
        return font_color_;
    }
    
    void TextViewBase::set_font_color(const Color & value) {
        font_color_ = value;
    }
    
    Text::Index TextViewBase::cursor_index() const {
        return cursor_index_;
    }
    
    void TextViewBase::set_cursor_index(const Text::Index & value) {
        Print(Format("cursor index: %s", value.ToString().c_str()));
        RHETORIC_ASSERT(text_->CheckIndex(value));
        cursor_index_ = value;
        cursor_blink_time_ = 0.0f;
    }

    bool TextViewBase::MoveCursorLeft() {
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
    
    bool TextViewBase::MoveCursorRight() {
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
    
    bool TextViewBase::MoveCursorUp() {
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
    
    bool TextViewBase::MoveCursorDown() {
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
    
    Size TextViewBase::MeasureOwnContent(NVGcontext * ctx, const MeasureQuery & query) const {
        auto layout = text_layouter_->Layout(ctx,
                                             text_,
                                             query.max_width());
        return layout->frame().size();
    }
    
    void TextViewBase::LayoutOwnContent(NVGcontext * ctx) {
        text_draw_info_ = text_layouter_->Layout(ctx,
                                                 text_,
                                                 Some(bounds().size().width()));
        float top = text_draw_info_->frame().origin().y();
        text_draw_info_->set_draw_offset(Vector2(0, -top));
    }
    
    void TextViewBase::DrawOwnContent(NVGcontext * ctx) {
        RHETORIC_ASSERT(text_draw_info_ != nullptr);
        
        NVGSetFillColor(ctx, font_color());
        
        text_layouter_->Draw(ctx, text_, text_draw_info_);
        
        if (_cursor_visible()) {
            if (cursor_blink_time_ <= 0.5f) {
                text_layouter_->DrawCursor(ctx, cursor_index(), text_draw_info_);
            }
        }
    }
    
    void TextViewBase::OnUpdateAnimation(float delta_time) {
        // TODO: timer
        cursor_blink_time_ += delta_time;
        if (cursor_blink_time_ > 1.0f) {
            cursor_blink_time_ = 0.0f;
        }
    }
    
    bool TextViewBase::OnMouseDownEvent(const MouseEvent & event) {
        if (!text_draw_info_) {
            return false;
        }
        
        auto position_index = text_draw_info_->GetIndexFor(event.pos());
        auto text_index = text_draw_info_->GetTextIndexFor(position_index, text_);
        
        set_cursor_index(text_index);
        cursor_x_ = GetCursorRect().origin().x();
        
        if (!_DoFocusByMouseDown()) {
            return false;
        }
        
        return true;
    }
    
    bool TextViewBase::OnKeyDownEvent(const KeyEvent & event) {
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
    
    bool TextViewBase::OnKeyRepeatEvent(const KeyEvent & event) {
        return OnKeyDownEvent(event);
    }
    
    void TextViewBase::OnCharEvent(const CharEvent & event) {
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
    
    bool TextViewBase::_cursor_visible() const {
        return cursor_visible_;
    }
    
    void TextViewBase::_set_cursor_visible(bool value) {
        cursor_visible_ = value;
    }
    
    Text::Index TextViewBase::GetTextIndexForLineIndexX(size_t line_index, float x) {
        RHETORIC_ASSERT(text_draw_info_ != nullptr);
        
        size_t char_index = text_draw_info_->GetCharIndexForX(line_index, x);
        TextDrawInfo::CharPositionIndex char_position_index(line_index, char_index);
        return text_draw_info_->GetTextIndexFor(char_position_index, text_);
    }
    
    Rect TextViewBase::GetCursorRect() const {
        RHETORIC_ASSERT(text_draw_info_ != nullptr);
        return text_layouter_->GetCursorRect(cursor_index_, text_draw_info_);
    }
}