#include "./scroll_bar.h"

#include "./window.h"

namespace oliview {
    void ScrollBar::Init(const Ptr<Application> & application, Axis axis) {
        View::Init(application);
        
        bar_width_ = 15.0f;
        border_color_ = Color(0.91f, 0.91f, 0.91f, 1.0f);
        knob_normal_color_ = Color(0.76f, 0.76f, 0.76f, 1.0f);
        knob_active_color_ = Color(0.49f, 0.49f, 0.49f, 1.0f);
        knob_margin_ = 3.0f;
        knob_min_height_ = 18.0f;
        
        axis_ = axis;
 
        content_size_ = 120.0f;
        visible_size_ = 120.0f;
        scroll_position_ = 0.0f;
        knob_active_ = false;
        
        set_background_color(Color(0.98f, 0.98f, 0.98f, 1.0f));
    }
    
    void ScrollBar::set_content_size(float value) {
        content_size_ = value;
        ClampScrollPosition();
        SetNeedsLayout();
    }
    
    void ScrollBar::set_visible_size(float value) {
        visible_size_ = value;
        ClampScrollPosition();
        SetNeedsLayout();
    }
    
    bool ScrollBar::ScrollTo(float value) {
        auto old = scroll_position_;
        scroll_position_ = value;
        ClampScrollPosition();
        SetNeedsLayout();
        return scroll_position_ != old;
    }
    
    bool ScrollBar::OnMouseDownEvent(const MouseEvent & event) {
        //  TODO: PageUp/Down (margin 10)
        
        auto pos = event.pos();
        if (GetKnobHitRect().Contains(pos)) {
            knob_active_ = true;
            
            auto range = GetKnobRange();
            switch (axis_) {
                case Axis::X:
                    knob_mouse_offset_ = Some(range.lower_bound() - pos.x());
                    break;
                case Axis::Y:
                    knob_mouse_offset_ = Some(range.lower_bound() - pos.y());
                    break;
            }
            
            return true;
        }
        
        return false;
    }
    
    void ScrollBar::OnMouseMoveEvent(const MouseEvent & event) {
        if (knob_mouse_offset_) {
            float y = event.pos().get(AxisToIndex(axis_)) + *knob_mouse_offset_;
            if (ScrollTo(GetScrollPositionForPosition(y))) {
                // TODO event
            }
        } else {
            UpdateKnobActive(event.pos());
        }
    }

    void ScrollBar::OnMouseUpEvent(const MouseEvent &) {
        OnMouseCancelEvent();
    }
    
    void ScrollBar::OnMouseCancelEvent() {
        EndKnobDragging();
    }
    
    bool ScrollBar::OnScrollEvent(const ScrollEvent & event) {
        float delta = event.scroll().get(AxisToIndex(axis_));
        
        if (ScrollTo(scroll_position() + delta)) {
            // TODO event
            return true;
        }
        
        return false;
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
        auto knob_range = GetKnobRange();
        
        switch (axis_) {
            case Axis::X: {
                //TODO
                break;
            }
            case Axis::Y: {
                auto bounds = this->bounds();
                float top = bounds.origin().y();
                float left = bounds.origin().x();
                float right = left + bar_width_;
                float bottom = bounds.size().height();
                
                nvgBeginPath(ctx);
                NVGAddRectPath(ctx, Rect(Vector2(left, top), Size(1, bottom - top)));
                NVGAddRectPath(ctx, Rect(Vector2(right - 1, top), Size(1, bottom - top)));
                NVGSetFillColor(ctx, border_color_);
                nvgFill(ctx);
                
                nvgBeginPath(ctx);
                NVGAddRoundRectPath(ctx, Rect(Vector2(left + 4.0f, knob_range.lower_bound()),
                                              Size(8.0f, knob_range.count())), 4.0f);
                if (knob_active_) {
                    NVGSetFillColor(ctx, knob_active_color_);
                } else {
                    NVGSetFillColor(ctx, knob_normal_color_);
                }
                nvgFill(ctx);
                
                break;
            }
        }
    }
    
    void ScrollBar::ClampScrollPosition() {
        float x = scroll_position_;
        x = Clamp(x, MakeRange(0.0f, content_size_ - visible_size_));
        scroll_position_ = x;
    }
    
    Range<float> ScrollBar::GetBarSpaceRange() const {
        switch (axis_) {
            case Axis::X: {
                float width = std::max(0.0f, bounds().size().width() - 2 * knob_margin_);
                float left = bounds().origin().x() + knob_margin_;
                return MakeRange(left, left + width);
            }
            case Axis::Y: {
                float height = std::max(0.0f, bounds().size().height() - 2 * knob_margin_);
                float top = bounds().origin().y() + knob_margin_;
                return MakeRange(top, top + height);
            }
        }
        RHETORIC_FATAL("never");
    }
    
    Range<float> ScrollBar::GetKnobRange() const {
        float top_rate = 0;
        float bottom_rate = 0;
        if (content_size_ > 0.0f) {
            top_rate = scroll_position_ / content_size_;
            bottom_rate = (scroll_position_ + visible_size_) / content_size_;
        }
        
        auto space_range = GetBarSpaceRange();
        float top = space_range.Blend(top_rate);
        float bottom = space_range.Blend(bottom_rate);
        bottom = std::max(bottom, top + knob_min_height_);
        return MakeRange(top, bottom);
    }
    
    Rect ScrollBar::GetKnobHitRect() const {
        auto range = GetKnobRange();
        auto bounds = this->bounds();
        switch (axis_) {
            case Axis::X: {
                return Rect(Vector2(range.lower_bound(), bounds.origin().y()),
                            Size(range.count(), bounds.size().height()));
            }
            case Axis::Y: {
                return Rect(Vector2(bounds.origin().x(), range.lower_bound()),
                            Size(bounds.size().width(), range.count()));
            }
        }
        RHETORIC_FATAL("never");
    }
    
    float ScrollBar::GetScrollPositionForPosition(float pos) {
        float rate = GetRate<float>(pos, GetBarSpaceRange());
        return rate * content_size_;
    }
    
    void ScrollBar::UpdateKnobActive(const Vector2 & pos) {
        if (bounds().Contains(pos)) {
            knob_active_ = true;
        } else {
            knob_active_ = false;
        }
    }
    
    void ScrollBar::EndKnobDragging() {
        knob_mouse_offset_ = None();
        knob_active_ = false;
        auto w = window();
        if (w) {
            auto pos = ConvertPointFromWindow(w->mouse_position());
            UpdateKnobActive(pos);
        }
    }
}
