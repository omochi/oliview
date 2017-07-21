#pragma once

#include "./common.h"

class Window5 : public Window {
public:
    virtual void Init(const Ptr<Application> & app) override;
    virtual void LayoutContentView(NVGcontext * ctx, const Ptr<WindowContentView> & view) override;
private:
    Ptr<TextBox> text_box_;
};
