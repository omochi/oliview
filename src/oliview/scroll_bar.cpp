#include "./scroll_bar.h"

namespace oliview {
    ScrollBar::ScrollBar():
    bar_width_(15)
    {}
    
    void ScrollBar::Init(const Ptr<Application> & application, Axis axis) {
        View::Init(application);
        
        axis_ = axis;
    }
    
    void ScrollBar::set_content_size(float value) {
        content_size_ = value;
        
        SetNeedsLayout();
    }
    
    Size ScrollBar::MeasureContent(NVGcontext * ctx, const MeasureQuery & query) const {
        RHETORIC_UNUSED(ctx);
        
        switch (axis_) {
            case Axis::X: {
                float width = 120;
                float height = bar_width_;
                if (query.max_width()) {
                    width = std::min(width, *query.max_width());
                }
                return Size(width, height);
            }
            case Axis::Y: {
                float width = bar_width_;
                float height = 120;
                if (query.max_height()) {
                    height = std::min(height, *query.max_height());
                }
                return Size(width, height);
            }
        }
        
        RHETORIC_FATAL("never");
    }
    
    void ScrollBar::LayoutContent(NVGcontext * ctx) {
        RHETORIC_UNUSED(ctx);
    }
    
    void ScrollBar::DrawContent(NVGcontext * ctx) {
        switch (axis_) {
            case Axis::X: {
                
                break;
            }
            case Axis::Y: {
                auto bounds = this->bounds();
                float top = bounds.origin().y();
                float left = bounds.origin().x();
                float right = left + bar_width_;
                float bottom = bounds.size().height();
                
                Color bg_color(0.98f, 0.98f, 0.98f, 1.0f);
                nvgBeginPath(ctx);
                NVGAddRectPath(ctx, Rect(Vector2(left, top), Size(right - left, bottom - top)));
                NVGSetFillColor(ctx, bg_color);
                nvgFill(ctx);
                
                Color border_color(0.91f, 0.91f, 0.91f, 1.0f);
                nvgBeginPath(ctx);
                NVGAddRectPath(ctx, Rect(Vector2(left, top), Size(1, bottom - top)));
                NVGAddRectPath(ctx, Rect(Vector2(right - 1, top), Size(1, bottom - top)));
                NVGSetFillColor(ctx, border_color);
                nvgFill(ctx);
                
                Color bar_color(0.76f, 0.76f, 0.76f, 1.0f);
//                Color bar_color(0.49f, 0.49f, 0.49f, 1.0f);
                nvgBeginPath(ctx);
                NVGAddRoundRectPath(ctx, Rect(Vector2(left + 4.0f, top + 3.0f), Size(8.0f, 40.0f)), 4.0f);
                NVGSetFillColor(ctx, bar_color);
                nvgFill(ctx);
                
                break;
            }
        }
    }
}
