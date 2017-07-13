#include "./window_root_view.h"

#include "./window.h"

namespace oliview {
    void WindowRootView::Init(const Ptr<Application> & application) {
        View::Init(application);
    }
    
    bool WindowRootView::OnKeyEvent(const KeyEvent & event) {
        auto w = window();
        
        if (event.type() == KeyEventType::Down) {
            if (event.key() == GLFW_KEY_TAB) {
                bool shift = (event.modifier() & GLFW_MOD_SHIFT) != 0;
                if (!shift) {
                    w->FocusNext();
                    return true;
                } else {
                    w->FocusPrev();
                    return true;
                }
            }
        }
        
        return false;
    }
}
