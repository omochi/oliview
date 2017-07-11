#include "./view.h"

#include "./application.h"
#include "./window.h"

namespace oliview {
    View::DrawInfo::DrawInfo():
    window_transform(Matrix3x3::Identity())
    {}
    
    View::DrawCommand::DrawCommand():
    shadow(false)
    {}
    
    View::View():
    background_color_(1, 1, 1, 0),
    needs_layout_(true),
    self_layouting_(false),
    clipping_children_(false)
    {}
    
    View::~View() {}
    
    void View::Init(const Ptr<Application> & application) {
        application_ = application;
    }
    
    void View::AddChild(const Ptr<View> & child) {
        RHETORIC_ASSERT(child->parent() == nullptr);
        
        children_.push_back(child);
        child->_SetParent(shared_from_this());
        
        SetNeedsLayout();
    }

    void View::RemoveChild(const Ptr<View> & child) {
        while (true) {
            auto index = ArrayFindIndexEqR(children_, child);
            if (!index) {
                break;
            }
            RemoveChildAt(*index);
        }
    }

    void View::RemoveChildAt(size_t index) {
        auto child = children_[index];
        ArrayRemoveAt(&children_, index);
        child->_SetParent(nullptr);
        
        SetNeedsLayout();
    }
    
    void View::RemoveFromParent() {
        auto parent = this->parent();
        if (parent) {
            parent->RemoveChild(shared_from_this());
        }
    }

    void View::set_frame(const Rect & value) {
        frame_ = value;
        
        SetNeedsLayout();
    }
    
    Rect View::local_frame() const {
        return Rect(Vector2(0, 0), frame_.size());
    }

    void View::set_background_color(const Color & value) {
        background_color_ = value;
    }
    
    void View::set_clipping_children(bool value) {
        clipping_children_ = value;
    }
    
    void View::SetNeedsLayout() {
        needs_layout_ = true;
    }
    
    void View::set_children_layouter(const Ptr<Layouter> & value) {
        if (children_layouter_) {
            children_layouter_->_set_view(nullptr);
        }
        
        children_layouter_ = value;
        
        if (children_layouter_) {
            children_layouter_->_set_view(shared_from_this());
        }
        
        SetNeedsLayout();
    }
    
    Size View::Measure(NVGcontext * ctx, const MeasureQuery & query) const {
        Size size = MeasureContent(ctx, query);
        
        if (children_layouter_) {
            Size layouter_size = children_layouter_->Measure(ctx, query);
            
            size = size.GetMax(layouter_size);
        }
        
        return size;
    }
    
    Size View::MeasureContent(NVGcontext * ctx, const MeasureQuery & query) const {
        RHETORIC_UNUSED(ctx);
        
        if (children_layouter_) {
            return Size(0, 0);
        }
        
        float width = frame().size().width();
        float height = frame().size().height();
        
        if (query.max_width()) {
            width = std::min(width, *query.max_width());
        }
        if (query.max_height()) {
            height = std::min(height, *query.max_height());
        }
        
        return Size(width, height);
    }
    
    void View::LayoutContent(NVGcontext * ctx) {
        RHETORIC_UNUSED(ctx);
    }

    void View::DrawContent(NVGcontext * ctx) {
        RHETORIC_UNUSED(ctx);
    }
    
    Matrix3x3 View::local_transform() const {
        return Matrix3x3::Translation(frame_.origin());
    }
    
    Matrix3x3 View::window_transform() const {
        return draw_info_.window_transform;
    }
    
    Vector2 View::ConvertPointToWindow(const Vector2 & point) const {
        return point.ApplyTransform(window_transform());
    }
    
    Vector2 View::ConvertPointFromWindow(const Vector2 & point) const {
        auto m = window_transform().Invert();
        return point.ApplyTransform(m);
    }
    
    Vector2 View::ConvertPointToView(const Vector2 & point, const Ptr<View> & view) const {
        return view->ConvertPointFromWindow(ConvertPointToWindow(point));
    }
    
    Vector2 View::ConvertPointFromView(const Vector2 & point, const Ptr<View> & view) const {
        return ConvertPointFromWindow(view->ConvertPointToWindow(point));
    }
    
    bool View::IsPointInside(const Vector2 & point) const {
        return local_frame().IsPointInside(point);
    }
    
    Ptr<View> View::HitTest(const MouseEvent & event) {
        bool skip_children = false;
        if (clipping_children()) {
            if (!IsPointInside(event.pos())) {
                skip_children = true;
            }
        }
        
        if (!skip_children) {
            auto children = ArrayReversed(this->children());
            for (auto & child : children) {
                Vector2 pos_in_child = ConvertPointToView(event.pos(), child);
                MouseEvent child_event = event;
                child_event.set_pos(pos_in_child);
                auto ret = child->HitTest(child_event);
                if (ret) {
                    return ret;
                }
            }
        }
        
        return nullptr;
    }
    
    void View::OnMouseDownEvent(const MouseEvent & event) {
        RHETORIC_UNUSED(event);
    }
    
    void View::OnMouseMoveEvent(const MouseEvent & event) {
        RHETORIC_UNUSED(event);
    }
    
    void View::OnMouseUpEvent(const MouseEvent & event) {
        RHETORIC_UNUSED(event);
    }
    
    void View::OnMouseCancelEvent() {
    }
    
    bool View::_InvokeLayout(NVGcontext * ctx) {
        bool updated = false;
        
        if (needs_layout_) {
            needs_layout_ = false;
            
            self_layouting_ = true;
            Layout(ctx);
            self_layouting_ = false;
            
            updated = true;
        }
        
        for (auto & child : children()) {
            if (child->_InvokeLayout(ctx)) {
                updated = true;
            }
        }
        
        return updated;
    }

    void View::_PrepareToDraw(const DrawInfo & info) {
        draw_info_ = info;
        
        auto wtr = draw_info_.window_transform;
        wtr = local_transform() * wtr;
        draw_info_.window_transform = wtr;
        
        Rect frame_in_window = Rect(Vector2(), frame().size()).ApplyTransform(wtr);
        draw_info_.content_clip_frame = frame_in_window.GetIntersection(draw_info_.parent_clip_frame);

        DrawInfo child_info = draw_info_;
        
        if (clipping_children_) {
            child_info.parent_clip_frame = child_info.content_clip_frame;
        }
        
        for (auto & child : children_) {
            child->_PrepareToDraw(child_info);
        }
    }
    
    void View::_InvokeDraw(NVGcontext * ctx, bool shadow) {
        if (shadow) {
            DrawShadow(ctx);
            return;
        }
                
        nvgSave(ctx);
        NVGSetScissor(ctx, draw_info_.content_clip_frame);
        NVGTransform(ctx, draw_info_.window_transform);
        DrawBackground(ctx);
        DrawContent(ctx);
        nvgRestore(ctx);
    }

    void View::_CollectDrawCommand(std::vector<DrawCommand> * commands, bool shadow) {
        if (shadow) {
            for (auto & child : children_) {
                DrawCommand command;
                command.view = child;
                command.shadow = true;
                commands->push_back(command);
            }
        }
        
        for (auto & child : children_) {
            DrawCommand command;
            command.view = child;
            command.shadow = false;
            commands->push_back(command);
        }

        for (auto & child : children_) {
            child->_CollectDrawCommand(commands, shadow);
        }
    }
    
    void View::_SetParent(const Ptr<View> & parent) {
        RHETORIC_ASSERT(!self_layouting_);
        
        if (parent) {
            RHETORIC_ASSERT(application() == parent->application());
        }
        
        parent_ = parent;
        
        auto old_window = this->window();
        if (old_window) {
            InvokeWindowOnRemoveView(old_window);
        }
        
        Ptr<Window> new_window;
        if (parent) {
            new_window = parent->window();
        }
        
        _SetWindow(new_window);

        if (new_window) {
            InvokeWindowOnAddView(new_window);
        }
    }

    void View::_SetWindow(const Ptr<Window> & window) {
        if (window) {
            RHETORIC_ASSERT(application() == window->application());
        }
    
        window_ = window;

        for (auto & child : children_) {
            child->_SetWindow(window);
        }
    }
    
    MouseEvent View::_ConvertMouseEventFromWindow(const MouseEvent & event) const {
        MouseEvent ret = event;
        ret.set_pos(ConvertPointFromWindow(event.pos()));
        return ret;
    }
    
    void View::Layout(NVGcontext * ctx) {
        LayoutContent(ctx);
        
        if (children_layouter_) {
            children_layouter_->Layout(ctx);
        }
    }
    
    void View::DrawBackground(NVGcontext * ctx) {
        nvgBeginPath(ctx);
        NVGAddRectPath(ctx, Rect(Vector2(), frame_.size()));
        NVGSetFillColor(ctx, background_color_);
        nvgFill(ctx);
    }
    
    void View::DrawShadow(NVGcontext * ctx) {
        RHETORIC_UNUSED(ctx);
    }
    
    void View::InvokeWindowOnAddView(const Ptr<Window> & window) {
        window->_OnAddView(shared_from_this());
        
        for (auto & child : children()) {
            child->InvokeWindowOnAddView(window);
        }
    }
    
    void View::InvokeWindowOnRemoveView(const Ptr<Window> & window) {
        for (auto & child : children()) {
            child->InvokeWindowOnRemoveView(window);
        }
        
        window->_OnRemoveView(shared_from_this());
    }
    
    
}
