#include "./window_content_view.h"

#include "./window.h"

namespace oliview {
    void WindowContentView::Layout(NVGcontext * ctx) {
        window()->LayoutContentView(ctx, shared_from_this());
    }
}

