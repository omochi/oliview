#include "./window_root_view.h"

#include "./window.h"

namespace oliview {
    void WindowRootView::Layout(NVGcontext *) {
        auto cv = window()->content_view();
        cv->set_frame(bounds());
    }
    
    bool WindowRootView::OnMouseDownEvent(const MouseEvent & event) {
        RHETORIC_UNUSED(event);
        window()->UnfocusView();
        return true;
    }
    
    bool WindowRootView::OnKeyDownEvent(const KeyEvent & event) {
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
            } else if (event.key() == GLFW_KEY_W) {
                bool cmd = (event.modifier() & GLFW_MOD_SUPER) != 0;
                if (cmd) {
                    w->Close();
                    return true;
                }
            }
        }
        
        return false;
    }
}
