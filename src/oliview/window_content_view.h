#pragma once

#include "./view.h"

namespace oliview {
    class Window;
    
    class WindowContentView : public View {
    public:
        RHETORIC_SUBCLASS_SHARED_FROM_THIS(WindowContentView, View)
        
        virtual void LayoutOwnContent(NVGcontext * ctx) override;
    private:
        
    };
}
