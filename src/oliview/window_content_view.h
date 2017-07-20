#pragma once

#include "./view.h"

namespace oliview {
    class Window;
    
    class WindowContentView : public View {
    public:
        virtual void Layout(NVGcontext * ctx) override;
    private:
        
    };
}
