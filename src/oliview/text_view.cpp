#include "./text_view.h"

#include "./application.h"

namespace oliview {
    void TextView::Init(const Ptr<Application> & application) {
        TextViewBase::Init(application);
        
        set_background_color(Color(1.0f, 1.0f, 1.0f, 1.0f));
        set_editable(false);
    }
    
    void TextView::OnFocus() {
        _set_cursor_visible(true);
    }
    
    void TextView::OnUnfocus() {
        _set_cursor_visible(false);
    }
    
    bool TextView::_DoFocusByMouseDown() {
        this->Focus();
        return true;
    }
}
