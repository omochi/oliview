#pragma once

#include "./view.h"

namespace oliview {
    class Window;
    
    class WindowRootView : public View {
    public:
        virtual void Layout(NVGcontext * ctx) override;
        
        virtual bool OnMouseDownEvent(const MouseEvent & event) override;
        virtual bool OnKeyDownEvent(const KeyEvent & event) override;
    };
}
