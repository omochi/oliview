#include "./text_box_content_view.h"

#include "./text_box.h"

namespace oliview {
    void TextBoxContentView::Init(const Ptr<Application> & application,
                                  const Ptr<TextBox> & text_box)
    {
        View::Init(application);
        text_box_ = text_box;
    }
    
    Ptr<TextBox> TextBoxContentView::text_box() const {
        return text_box_.lock();
    }
    
    void TextBoxContentView::DrawOwnContent(NVGcontext * ctx) {
        text_box()->_DrawTextViewContent(ctx, shared_from_this());
    }
    
    bool TextBoxContentView::OnMouseDownEvent(const MouseEvent & event) {
        return text_box()->_OnTextViewMouseDownEvent(event);
    }
    
    void TextBoxContentView::OnMouseMoveEvent(const MouseEvent &) {}
    void TextBoxContentView::OnMouseUpEvent(const MouseEvent &) {}
    void TextBoxContentView::OnMouseCancelEvent() {}
}
