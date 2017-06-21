#include "./view.h"

#include "./window.h"

namespace oliview {
    View::DrawInfo::DrawInfo():
    window_transform(Matrix3x3::Identity())
    {
    }

    View::View():
    background_color_(1, 1, 1, 0)
    {
    }
    
    View::~View() {
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
    }

    Ptr<Window> View::window() const {
        return window_.lock();
    }
    
    Ptr<Application> View::application() const {
        return window()->application();
    }

    Rect View::frame() const {
        return frame_;
    }

    void View::set_frame(const Rect & value) {
        frame_ = value;
    }

    Color View::background_color() const {
        return background_color_;
    }

    void View::set_background_color(const Color & value) {
        background_color_ = value;
    }

    void View::PreDraw(const DrawInfo & info) {
        draw_info_ = info;

        auto tr = draw_info_.window_transform;
        tr *= Matrix3x3::Translation(frame_.origin());
        draw_info_.window_transform = tr;

        for (auto & child : children_) {
            child->PreDraw(draw_info_);
        }
    }

    void View::Draw(NVGcontext * ctx) {
        nvgSave(ctx);

        auto tr = draw_info_.window_transform;
        tr = tr.Transpose();
        nvgTransform(ctx,
                     tr.get(0, 0), tr.get(1, 0),
                     tr.get(0, 1), tr.get(1, 1),
                     tr.get(0, 2), tr.get(1, 2));
        DrawContent(ctx);
        nvgRestore(ctx);

        for (auto & child : children_) {
            child->Draw(ctx);
        }
    }

    void View::DrawContent(NVGcontext * ctx) {
        nvgBeginPath(ctx);
        nvgRect(ctx,
                0,
                0,
                frame_.size().x(),
                frame_.size().y());
        nvgFillColor(ctx, background_color_.ToNanoVG());
        nvgFill(ctx);
    }

    void View::_SetParent(const Ptr<View> & parent) {
        parent_ = parent;

        if (parent) {
            _SetWindow(parent->window());
        } else {
            _SetWindow(nullptr);
        }
    }

    void View::_SetWindow(const Ptr<Window> & window) {
        window_ = window;

        for (auto & child : children_) {
            child->_SetWindow(window);
        }
    }

}
