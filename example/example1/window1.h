#pragma once

#include "./common.h"

class Window1 : public Window {
public:
    virtual void Init(const Ptr<Application> & app) override;
    virtual void OnKeyDownEvent(const KeyEvent & event) override;
    
    virtual void LayoutContentView(NVGcontext * ctx, const Ptr<WindowContentView> & view) override;
private:
    int test_stage_;
    
    Ptr<TextBox> text_box_;
    Ptr<ScrollBar> scroll_bar_;
    Ptr<ScrollView> scroll_view_;
};

class Window1View : public View {
public:
//    virtual bool OnKeyEvent(const KeyEvent & event) override;
};
