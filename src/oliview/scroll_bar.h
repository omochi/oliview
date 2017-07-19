#pragma once

#include "./dependency.h"

#include "./axis.h"
#include "./mouse_event.h"
#include "./view.h"

namespace oliview {
    class Application;
    
    class ScrollBar : public View {
    public:        
        virtual void Init(const Ptr<Application> &) override RHETORIC_UNAVAILABLE {}
        
        void Init(const Ptr<Application> & application, Axis axis);
        
        RHETORIC_GETTER(Axis, axis)
        
        RHETORIC_GETTER(float, content_size)
        void set_content_size(float value);
        
        RHETORIC_GETTER(float, visible_size)
        void set_visible_size(float value);
        
        RHETORIC_GETTER(float, scroll_position)
        bool ScrollTo(float value);
        bool ScrollByPageUp();
        bool ScrollByPageDown();
        
        RHETORIC_ACCESSOR(std::function<void(float)>, scroll_handler)
        
        virtual bool OnMouseDownEvent(const MouseEvent & event) override;
        virtual void OnMouseMoveEvent(const MouseEvent & event) override;
        virtual void OnMouseUpEvent(const MouseEvent & event) override;
        virtual void OnMouseCancelEvent() override;
        
        virtual bool OnScrollEvent(const ScrollEvent & event) override;
        
        virtual Size MeasureContent(NVGcontext * ctx, const MeasureQuery & query) const override;
        virtual void LayoutContent(NVGcontext * ctx) override;
        virtual void DrawContent(NVGcontext * ctx) override;
    private:
        void ClampScrollPosition();
        
        Range<float> GetBarSpaceRange() const;
        Range<float> GetKnobRange() const;
        Rect GetKnobHitRect() const;
        float GetScrollPositionForPosition(float pos);
        
        void UpdateKnobActive(const Vector2 & pos);
        void EndKnobDragging();
        
        Axis axis_;
        float content_size_;
        float visible_size_;
        float scroll_position_;
        bool knob_active_;
        Option<float> knob_mouse_offset_;
        std::function<void(float)> scroll_handler_;
        
        float bar_width_;
        Color border_color_;
        Color knob_normal_color_;
        Color knob_active_color_;
        float knob_margin_;
        float knob_min_height_;
    };
}
