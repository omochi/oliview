#include "./layouter.h"

#include "./view.h"

namespace oliview {
    Layouter::~Layouter() {}
    
    Ptr<View> Layouter::view() const {
        return view_.lock();
    }
    
    void Layouter::SetNeedsLayout() const {
        auto view = this->view();
        if (view) {
            view->SetNeedsLayout();
        }
    }
    
    void Layouter::_set_view(const Ptr<View> & view) {
        view_ = view;
    }
}
