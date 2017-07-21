#include "./window_content_view.h"

#include "./window.h"

namespace oliview {
    void WindowContentView::LayoutOwnContent(NVGcontext * ctx) {
        window()->LayoutContentView(ctx, shared_from_this());
    }
}

