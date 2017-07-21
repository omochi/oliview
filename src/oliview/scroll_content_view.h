#pragma once

#include "./view.h"

namespace oliview {
    class ScrollView;
    
    class ScrollContentView : public View {
    public:
        virtual void Init(const Ptr<Application> &) override RHETORIC_UNAVAILABLE {}
        
        void Init(const Ptr<Application> & application,
                  const Ptr<ScrollView> & scroll_view);
        
        RHETORIC_SUBCLASS_SHARED_FROM_THIS(ScrollContentView, View)
        
        Ptr<ScrollView> scroll_view() const;
        
        virtual void LayoutOwnContent(NVGcontext * ctx) override;
    private:
        WeakPtr<ScrollView> scroll_view_;
    };
}
