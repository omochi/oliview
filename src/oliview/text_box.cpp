#include "./text_box.h"

#include "./application.h"

namespace oliview {    
    void TextBox::Init(const Ptr<Application> & application) {
        ScrollView::Init(application);
        
        text_view_ = OLIVIEW_INIT(TextBoxTextView, application, shared_from_this());
        content_view()->AddChild(text_view_);
        text_view_->set_focusable(false);
//        text_view_->set_background_color(Color(1.0f, 0.7f, 1.0f, 1.0f));
        
        set_background_color(Color(1.0f, 1.0f, 1.0f, 1.0f));
        set_focusable(true);
    }
    
    TextBox::~TextBox() {}
    
    bool TextBox::editable() const {
        return text_view_->editable();
    }
    
    void TextBox::set_editable(bool value) {
        text_view_->set_editable(value);
    }
    
    Ptr<const Text> TextBox::text() const {
        return text_view_->text();
    }
    
    void TextBox::set_text(const Ptr<const Text> & value) {
        text_view_->set_text(value);
        
        SetNeedsLayout();
    }
    
    void TextBox::InsertTextAt(const Text::Index & index,
                               const Ptr<const Text> & text,
                               Text::Index * end_index)
    {
        text_view_->InsertTextAt(index, text, end_index);
        SetNeedsLayout();
    }
    
    void TextBox::DeleteTextAt(const Text::Index & begin,
                               const Text::Index & end)
    {
        text_view_->DeleteTextAt(begin, end);        
        SetNeedsLayout();
    }
    
    Ptr<Font> TextBox::font() const {
        return text_view_->font();
    }
    
    void TextBox::set_font(const Ptr<Font> & value) {
        text_view_->set_font(value);
        SetNeedsLayout();
    }
    
    float TextBox::font_size() const {
        return text_view_->font_size();
    }
    
    void TextBox::set_font_size(float value) {
        text_view_->set_font_size(value);
        SetNeedsLayout();
    }
    
    Color TextBox::font_color() const {
        return text_view_->font_color();
    }
    
    void TextBox::set_font_color(const Color & value) {
        text_view_->set_font_color(value);
    }
    
    TextAlignment TextBox::text_alignment() const {
        return text_view_->text_alignment();
    }
    
    void TextBox::set_text_alignment(TextAlignment value) {
        text_view_->set_text_alignment(value);
        SetNeedsLayout();
    }

    Text::Index TextBox::cursor_index() const {
        return text_view_->cursor_index();
    }
    
    void TextBox::set_cursor_index(const Text::Index & value) {
        text_view_->set_cursor_index(value);
    }

    bool TextBox::MoveCursorLeft() {
        return text_view_->MoveCursorLeft();
    }
    
    bool TextBox::MoveCursorRight() {
        return text_view_->MoveCursorRight();
    }

    bool TextBox::MoveCursorUp() {
        return text_view_->MoveCursorUp();
    }
    
    bool TextBox::MoveCursorDown() {
        return text_view_->MoveCursorDown();
    }
    
    Size TextBox::MeasureScrollContentView(NVGcontext * ctx,
                                           const Ptr<ScrollContentView> & view,
                                           const Size & visible_size) const
    {
        RHETORIC_UNUSED(view);
        MeasureQuery query;
        query.set_max_width(Some(visible_size.width()));
        Size text_view_size = text_view_->Measure(ctx, query);
        return text_view_size;
    }
    
    void TextBox::LayoutScrollContentView(NVGcontext * ctx,
                                          const Ptr<ScrollContentView> & view)
    {
        RHETORIC_UNUSED(ctx);
        RHETORIC_UNUSED(view);
        text_view_->set_frame(Rect(Vector2(), content_size().GetMax(visible_size())));
    }

    bool TextBox::OnKeyDownEvent(const KeyEvent & event) {
        return text_view_->OnKeyDownEvent(event);
    }
    
    void TextBox::OnCharEvent(const CharEvent & event) {
        text_view_->OnCharEvent(event);
    }
    
    void TextBox::OnFocus() {
        text_view_->_set_cursor_visible(true);
    }
    
    void TextBox::OnUnfocus() {
        text_view_->_set_cursor_visible(false);
    }
    
    void TextBoxTextView::Init(const Ptr<Application> & application,
                               const Ptr<TextBox> & owner)
    {
        TextViewBase::Init(application);
        owner_ = owner;
    }
    
    bool TextBoxTextView::_DoFocusByMouseDown() {
        owner()->Focus();
        return true;
    }
    
}
