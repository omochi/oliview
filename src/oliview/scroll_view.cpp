#include "./scroll_view.h"

namespace oliview {
    void ScrollView::Init(const Ptr<Application> & app) {
        View::Init(app);
        
        auto_content_size_enabled_ = true;
        
        set_background_color(Color(1.0f, 1.0f, 1.0f, 1.0f));

        content_view_ = OLIVIEW_INIT(ScrollContentView, app);
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
        ClampScrollPosition();
        SetNeedsLayout();
    }
    
    void ScrollView::set_auto_content_size_enabled(bool value) {
        auto_content_size_enabled_ = value;
        
        SetNeedsLayout();
    }
    
    bool ScrollView::ScrollTo(const Vector2 & position) {
        auto old = scroll_position_;
        
        scroll_position_ = position;
        ClampScrollPosition();
        
        if (scroll_position_ == old) {
            return false;
        }
        
        y_bar_->ScrollTo(scroll_position_.y());
        x_bar_->ScrollTo(scroll_position_.x());
        
        SetNeedsLayout();
        
        return true;
    }
    
    Size ScrollView::Measure(NVGcontext * ctx, const MeasureQuery & query) const {
        return MeasureOwnContent(ctx, query);
    }
    
    void ScrollView::Layout(NVGcontext * ctx) {
        auto layouter = content_view_->children_layouter();
        
        if (auto_content_size_enabled_) {
            if (layouter) {
                Size size = content_view_->frame().size();
                
                MeasureQuery query;
                Size children_size = layouter->Measure(ctx, query);
                size = size.GetMax(children_size);
                content_size_ = size;
            } else {
                Vector2 end = content_view_->frame().end();
                for (auto & child : content_view_->children()) {
                    end = end.GetMax(child->frame().end());
                }
                Size size(end);
                content_size_ = size;
            }
            ClampScrollPosition();
        }
        
        {
            auto bounds = this->bounds();
            
            float visible_height = bounds.size().height();
            float visible_width = bounds.size().width();
            
            visible_height -= 15.0f;
            visible_width -= 15.0f;
            
            content_view_->set_frame(Rect(bounds.origin(), Size(visible_width, visible_height)));
            content_view_->set_bounds(Rect(scroll_position_, Size(visible_width, visible_height)));
            
            y_bar_->set_frame(Rect(Vector2(bounds.end().x() - 15.0f, bounds.origin().y()),
                                   Size(15.0f, bounds.size().height() - 15.0f)));
            y_bar_->set_content_size(content_size_.height());
            y_bar_->set_visible_size(visible_height);
            
            x_bar_->set_frame(Rect(Vector2(0, bounds.end().y() - 15.0f),
                                   Size(bounds.size().width() - 15.0f, 15.0f)));
            x_bar_->set_content_size(content_size_.width());
            x_bar_->set_visible_size(visible_width);
        }
        
        layouter = children_layouter();
        if (layouter) {
            layouter->Layout(ctx);
        }
    }
    
    bool ScrollView::OnScrollEvent(const ScrollEvent & event) {
        Vector2 delta = event.scroll();
        
        if (ScrollTo(scroll_position() + delta)) {
            return true;
        }
        
        return false;
    }
    
    void ScrollView::ClampScrollPosition() {
        Size visible_size = frame().size();
        float x = Clamp(scroll_position_.x(),
                        MakeRange(0.0f, content_size_.width() - (visible_size.width() - 15.0f)));
        float y = Clamp(scroll_position_.y(),
                        MakeRange(0.0f, content_size_.height() - (visible_size.height() - 15.0f)));
        scroll_position_ = Vector2(x, y);
    }
    
    void ScrollContentView::Init(const Ptr<Application> & app) {
        View::Init(app);
        
    }

}

