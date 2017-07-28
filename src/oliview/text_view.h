#pragma once

#include "./text_view_base.h"

namespace oliview {
    /*
     see TextViewBase for API
     */
    class TextView : public TextViewBase {
    public:
        virtual void Init(const Ptr<Application> & application) override;
        virtual void InitAsLabel(const Ptr<Application> & application);
        
        RHETORIC_SUBCLASS_SHARED_FROM_THIS(TextView, TextViewBase)
        
        virtual void OnFocus() override;
        virtual void OnUnfocus() override;
        virtual bool _DoFocusByMouseDown() override;
    private:

    };
}
