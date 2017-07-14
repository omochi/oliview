#pragma once

#include "./view.h"

namespace oliview {
    class Window;
    
    class WindowRootView : public View {
    public:
        virtual void Init(const Ptr<Application> & application) override;
    private:        
        virtual bool OnMouseDownEvent(const MouseEvent & event) override;
        virtual bool OnKeyDownEvent(const KeyEvent & event) override;
    };
}
