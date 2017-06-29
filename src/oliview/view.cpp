#include "./view.h"

#include "./application.h"
#include "./window.h"

namespace oliview {
    View::DrawInfo::DrawInfo():
    window_transform(Matrix3x3::Identity())
    {
    }
    
    View::View(const Ptr<Application> & application):
    application_(application),
    background_color_(1, 1, 1, 0),
    needs_layout_(true)
    {
    }
    
    View::~View() {
    }
    
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

    Ptr<Window> View::window() const {
        return window_.lock();
    }

    Rect View::frame() const {
        return frame_;
    }

    void View::set_frame(const Rect & value) {
        frame_ = value;
        
        SetNeedsLayout();
    }

    Color View::background_color() const {
        return background_color_;
    }

    void View::set_background_color(const Color & value) {
        background_color_ = value;
    }
    
    void View::Layout() {
        if (layout_function_) {
            layout_function_();
        }
    }
    
    bool View::MayLayout() {
        if (!needs_layout_) {
            return false;
        }
        
        needs_layout_ = false;
        Layout();
        return true;
    }
    
    void View::SetNeedsLayout() {
        needs_layout_ = true;
    }
    
    void View::set_layout_function(const std::function<void()> & value) {
        layout_function_ = value;
        
        SetNeedsLayout();
    }

    void View::PreDraw(const DrawInfo & info) {
        draw_info_ = info;

        auto tr = draw_info_.window_transform;
        tr = Matrix3x3::Translation(frame_.origin()) * tr;
        draw_info_.window_transform = tr;

        for (auto & child : children_) {
            child->PreDraw(draw_info_);
        }
    }

    void View::Draw() {
        auto app = application();
        auto ctx = app->nvg_context();
        
        Rect content_clip = Rect(Vector2(0, 0), frame().size());
        content_clip = content_clip.ApplyTransform(draw_info_.window_transform);
        content_clip = content_clip.GetIntersection(draw_info_.clip_frame);
        
        nvgSave(ctx);
        nvgScissor(ctx,
                   content_clip.origin().x(),
                   content_clip.origin().y(),
                   content_clip.size().width(),
                   content_clip.size().height());

        auto tr = draw_info_.window_transform;
        tr = tr.Transpose();
        nvgTransform(ctx,
                     tr.get(0, 0), tr.get(1, 0),
                     tr.get(0, 1), tr.get(1, 1),
                     tr.get(0, 2), tr.get(1, 2));

        DrawContent();
        nvgRestore(ctx);

        for (auto & child : children_) {
            child->Draw();
        }
    }

    void View::DrawContent() {
        auto app = application();
        auto ctx = app->nvg_context();
        
        nvgBeginPath(ctx);
        nvgRect(ctx,
                0,
                0,
                frame_.size().width(),
                frame_.size().height());
        nvgFillColor(ctx, background_color_.ToNanoVG());
        nvgFill(ctx);
    }

    void View::_SetParent(const Ptr<View> & parent) {
        if (parent) {
            RHETORIC_ASSERT(application() == parent->application());
        }
        
        parent_ = parent;

        if (parent) {
            _SetWindow(parent->window());
        } else {
            _SetWindow(nullptr);
        }
        
        SetNeedsLayout();
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

    
}
