#pragma once

#include "./common.h"

class Window3 : public oliview::Window {
public:
    virtual void Init(const Ptr<Application> & app) override;
    
    virtual void OnBeginDraw(NVGcontext * ctx) override;
private:
    
};
