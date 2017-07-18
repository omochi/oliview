#pragma once

#include "./dependency.h"

#include "./axis.h"
#include "./view.h"

namespace oliview {
    class Application;
    
    class ScrollBar : public View {
    public:
        ScrollBar();
        
        // TODO: move to rhetoric
        virtual void Init(const Ptr<Application> & application) override __attribute__((unavailable)) {
            View::Init(application);
        }
        
        void Init(const Ptr<Application> & application, Axis axis);
        
        RHETORIC_GETTER(Axis, axis)
        
        RHETORIC_GETTER(float, content_size)
        void set_content_size(float value);
        
        virtual Size MeasureContent(NVGcontext * ctx, const MeasureQuery & query) const override;
        virtual void LayoutContent(NVGcontext * ctx) override;
        virtual void DrawContent(NVGcontext * ctx) override;
    private:
        Axis axis_;
        float content_size_;
        
        float bar_width_;
    };
}
