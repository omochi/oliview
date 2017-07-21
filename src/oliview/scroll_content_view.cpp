#include "./scroll_content_view.h"

#include "./scroll_view.h"

namespace oliview {
    void ScrollContentView::Init(const Ptr<Application> & application,
                                 const Ptr<ScrollView> & scroll_view)
    {
        View::Init(application);
        
        scroll_view_ = scroll_view;
    }
    
    Ptr<ScrollView> ScrollContentView::scroll_view() const {
        return scroll_view_.lock();
    }
    
    void ScrollContentView::LayoutOwnContent(NVGcontext * ctx) {
        scroll_view()->LayoutScrollContentView(ctx, shared_from_this());
    }
}
