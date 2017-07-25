#include "./scroll_view.h"

namespace oliview {
    void ScrollView::Init(const Ptr<Application> & app) {
        View::Init(app);
        
        auto_content_size_enabled_ = true;
        
        set_background_color(Color(1.0f, 1.0f, 1.0f, 1.0f));

        content_view_ = OLIVIEW_INIT(ScrollContentView, app, shared_from_this());
        AddChild(content_view_);
        content_view_->set_clipping_children(true);
        
        y_bar_ = OLIVIEW_INIT(ScrollBar, app, Axis::Y);
        AddChild(y_bar_);
        
        x_bar_ = OLIVIEW_INIT(ScrollBar, app, Axis::X);
        AddChild(x_bar_);
        
        y_bar_->set_scroll_handler([this](auto y) {
            ScrollTo(Vector2(scroll_position_.x(), y));
        });
        x_bar_->set_scroll_handler([this](auto x) {
            ScrollTo(Vector2(x, scroll_position_.y()));
        });
    }
    
    Ptr<View> ScrollView::content_view() const {
        return content_view_;
    }
    
    void ScrollView::set_content_size(const Size & value) {
        content_size_ = value;
        SetNeedsLayout();
    }
    
    void ScrollView::set_auto_content_size_enabled(bool value) {
        auto_content_size_enabled_ = value;
        SetNeedsLayout();
    }
    
    bool ScrollView::ScrollTo(const Vector2 & position) {
        auto updated = UpdateScrollPosition(position);
        if (!updated) {
            return false;
        }
        
        y_bar_->ScrollTo(scroll_position_.y());
        x_bar_->ScrollTo(scroll_position_.x());

        content_view_->set_bounds(Rect(scroll_position_, visible_size_));
        
        return true;
    }
    
    Size ScrollView::MeasureScrollContentView(NVGcontext * ctx,
                                              const Ptr<ScrollContentView> & view,
                                              const Size & visible_size) const
    {
        auto layouter = view->children_layouter();
        if (layouter) {
            MeasureQuery query;
            Size size = layouter->Measure(ctx, query);
            size = size.GetMax(visible_size);
            return size;
        } else {
            Vector2 end;
            for (auto & child : view->children()) {
                end = end.GetMax(child->frame().end());
            }
            Size size(end);
            size = size.GetMax(visible_size);
            return size;
        }
    }
    
    void ScrollView::LayoutScrollContentView(NVGcontext *,
                                             const Ptr<ScrollContentView> &)
    {}
        
    Size ScrollView::Measure(NVGcontext * ctx, const MeasureQuery & query) const {
        // cut measure chain
        return MeasureOwnContent(ctx, query);
    }
    
    void ScrollView::Layout(NVGcontext * ctx) {
        auto layouter = content_view_->children_layouter();
        
        auto bounds = this->bounds();
        
        auto bars_ret = ComputeBarsVisibility(ctx);

        y_bar_->set_visible(bars_ret.y_bar);
        x_bar_->set_visible(bars_ret.x_bar);
        content_size_ = bars_ret.content_size;
        visible_size_ = bars_ret.visible_size;
        
        content_view_->set_frame(Rect(bounds.origin(), visible_size_));
        
        bool updated = UpdateScrollPosition(scroll_position_);
        if (updated) {
            content_view_->set_bounds(Rect(scroll_position_, visible_size_));
        }

        if (y_bar_->visible()) {
            y_bar_->set_frame(Rect(Vector2(bounds.end().x() - 15.0f, bounds.origin().y()),
                                   Size(15.0f, visible_size_.height())));
            y_bar_->set_content_size(content_size_.height());
            y_bar_->set_visible_size(visible_size_.height());
        }
        
        if (x_bar_->visible()) {
            x_bar_->set_frame(Rect(Vector2(0, bounds.end().y() - 15.0f),
                                   Size(visible_size_.width(), 15.0f)));
            x_bar_->set_content_size(content_size_.width());
            x_bar_->set_visible_size(visible_size_.width());
        }
        
        if (updated) {
            // TODO: event
        }
        
        layouter = children_layouter();
        if (layouter) {
            layouter->Layout(ctx);
        }
    }
    
    bool ScrollView::OnScrollEvent(const ScrollEvent & event) {
        Vector2 delta = event.scroll();
        
        if (ScrollTo(scroll_position_ + delta)) {
            return true;
        }
        
        return false;
    }
    
    bool ScrollView::UpdateScrollPosition(const Vector2 & value) {
        auto old = scroll_position_;
        
        float x = Clamp(value.x(),
                        MakeRange(0.0f, content_size_.width() - visible_size_.width()));
        float y = Clamp(value.y(),
                        MakeRange(0.0f, content_size_.height() - visible_size_.height()));
        
        scroll_position_ = Vector2(x, y);
        
        return old != scroll_position_;
    }
    
    ScrollView::ComputeBarsVisibilityResult::ComputeBarsVisibilityResult():
    y_bar(false),
    x_bar(false)
    {}
    
    ScrollView::ComputeBarsVisibilityResult ScrollView::ComputeBarsVisibility(NVGcontext * ctx) const {
        auto bounds = this->bounds();
        
        bool y_bar = false;
        bool x_bar = false;
        Size visible_size = bounds.size();
        Size content_size = content_size_;
        
        while (true) {
            if (auto_content_size_enabled_) {
                content_size = MeasureScrollContentView(ctx, content_view_, visible_size);
            }
            if (!y_bar) {
                if (content_size.height() > visible_size.height()) {
                    y_bar = true;
                    visible_size.set_width(std::max(0.0f, visible_size.width() - 15.0f));
                    continue;
                }
            }
            if (!x_bar) {
                if (content_size.width() > visible_size.width()) {
                    x_bar = true;
                    visible_size.set_height(std::max(0.0f, visible_size.height() - 15.0f));
                    continue;
                }
            }
            break;
        }
        
        ComputeBarsVisibilityResult ret;
        ret.y_bar = y_bar;
        ret.x_bar = x_bar;
        ret.visible_size = visible_size;
        ret.content_size = content_size;
        return ret;
    }
    
    
}

