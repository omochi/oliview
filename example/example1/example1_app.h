#pragma once

#include <oliview/oliview.h>

using namespace oliview;

class Example1App : public Application {
public:
    virtual void OnInit() override;
    
private:
    void OpenWindow1();
    void OpenWindow2();
};
