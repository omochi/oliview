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
    
    View::View(const Ptr<Application> & application):
    application_(application),
    background_color_(1, 1, 1, 0),
    needs_layout_(true),
    self_layouting_(false),
    clipping_children_(false)
    {}
    
    View::~View() {}
    
    Ptr<Application> View::application() const {
        return application_.lock();
    }

    Ptr<View> View::parent() const {
        return parent_.lock();
    }

    std::vector<Ptr<View>> View::children() const {
        return children_;
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

    void View::RemoveChildAt(int index) {
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

    Ptr<Window> View::window() const {
        return window_.lock();
    }

    void View::set_frame(const Rect & value) {
        frame_ = value;
        
        SetNeedsLayout();
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
    
    void View::set_layouter(const Ptr<Layouter> & value) {
        layouter_ = value;
        
        if (layouter_) {
            layouter_->_set_owner(shared_from_this());
        }
        
        SetNeedsLayout();
    }
    
    void View::OnLayout() {
    }
    
    Size View::Measure(const MeasureQuery & query) const {
        if (layouter_) {
            return layouter_->Measure(query);
        }
        
        return OnMeasure(query);
    }
    
    Size View::OnMeasure(const MeasureQuery & query) const {
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

    void View::Draw() {
        auto app = application();
        auto ctx = app->nvg_context();
        
        nvgBeginPath(ctx);
        NVGRect(ctx, Rect(Vector2(), frame_.size()));
        nvgFillColor(ctx, background_color_.ToNanoVG());
        nvgFill(ctx);
    }
    
    bool View::_InvokeLayout() {
        bool updated = false;
        
        if (needs_layout_) {
            needs_layout_ = false;
            
            self_layouting_ = true;
            Layout();
            self_layouting_ = false;
            
            updated = true;
        }
        
        for (auto & child : children()) {
            if (child->_InvokeLayout()) {
                updated = true;
            }
        }
        
        return updated;
    }

    void View::_PreDraw(const DrawInfo & info) {
        draw_info_ = info;
        
        auto wtr = draw_info_.window_transform;
        wtr = Matrix3x3::Translation(frame_.origin()) * wtr;
        draw_info_.window_transform = wtr;
        
        Rect frame_in_window = Rect(Vector2(), frame().size()).ApplyTransform(wtr);
        draw_info_.content_clip_frame = frame_in_window.GetIntersection(draw_info_.parent_clip_frame);

        DrawInfo child_info = draw_info_;
        
        if (clipping_children_) {
            child_info.parent_clip_frame = child_info.content_clip_frame;
        }
        
        for (auto & child : children_) {
            child->_PreDraw(child_info);
        }
    }
    
    void View::_InvokeDraw(bool shadow) {
        if (shadow) {
            DrawShadow();
            return;
        }
        
        auto app = application();
        auto ctx = app->nvg_context();
        
        nvgSave(ctx);
        NVGScissor(ctx, draw_info_.content_clip_frame);
        NVGTransform(ctx, draw_info_.window_transform);
        Draw();
        nvgRestore(ctx);
    }

    void View::_CollectDrawCommand(std::list<DrawCommand> * commands) {
        for (auto & child : children_) {
            DrawCommand command;
            command.view = child;
            command.shadow = true;
            commands->push_back(command);
        }
        
        for (auto & child : children_) {
            DrawCommand command;
            command.view = child;
            command.shadow = false;
            commands->push_back(command);
        }

        for (auto & child : children_) {
            child->_CollectDrawCommand(commands);
        }
    }
    
    void View::_SetParent(const Ptr<View> & parent) {
        RHETORIC_ASSERT(!self_layouting_);
        
        if (parent) {
            RHETORIC_ASSERT(application() == parent->application());
        }
        
        parent_ = parent;

        if (parent) {
            _SetWindow(parent->window());
        } else {
            _SetWindow(nullptr);
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
    
    void View::Layout() {
        if (layouter_) {
            layouter_->Layout();
            return;
        }
        
        OnLayout();
    }
    
    void View::DrawShadow() {
        
    }
    
    
}
