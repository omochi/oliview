#pragma once

#include "./common.h"

class Window6 : public Window {
public:
    virtual void Init(const Ptr<Application> & app) override;
    virtual void LayoutContentView(NVGcontext * ctx, const Ptr<WindowContentView> & view) override;
private:
    Ptr<TextView> text_view_;
};
