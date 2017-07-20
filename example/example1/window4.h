#pragma once

#include "./common.h"

class Window4 : public Window {
public:
    virtual void Init(const Ptr<Application> & app) override;
    virtual void LayoutContentView(NVGcontext * ctx, const Ptr<View> & view) override;
private:
    Ptr<ScrollView> scroll_view_;
};
