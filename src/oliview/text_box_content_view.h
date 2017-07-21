#pragma once

#include "./view.h"

namespace oliview {
    class TextBox;
    
    class TextBoxContentView : public View {
    public:
        virtual void Init(const Ptr<Application> &) override RHETORIC_UNAVAILABLE {}
        
        void Init(const Ptr<Application> & application,
                  const Ptr<TextBox> & text_box);
        
        RHETORIC_SUBCLASS_SHARED_FROM_THIS(TextBoxContentView, View)
        
        Ptr<TextBox> text_box() const;
        
        virtual void DrawOwnContent(NVGcontext * ctx) override;
        
        virtual bool OnMouseDownEvent(const MouseEvent & event) override;
        virtual void OnMouseMoveEvent(const MouseEvent & event) override;
        virtual void OnMouseUpEvent(const MouseEvent & event) override;
        virtual void OnMouseCancelEvent() override;
    private:
        WeakPtr<TextBox> text_box_;
    };
}
