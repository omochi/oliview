#include "./view.h"

#include "./window.h"

namespace oliview {
    View::View():
    transform_(Matrix3x3::Identity()),
    window_transform_dirty_(true),
    window_transform_(Matrix3x3::Identity()),
    background_color_(1, 1, 1, 0)
    {
    }

    Ptr<View> View::parent() const {
        return parent_.lock();
    }

    const std::vector<Ptr<View>> & View::children() const {
        return children_;
    }

    void View::AddChild(const Ptr<View> & child) {
        RHETORIC_ASSERT(child->parent() == nullptr);
        children_.push_back(child);
        child->SetParentInternal(shared_from_this());
    }

    void View::RemoveChild(const Ptr<View> & child) {
        while (true) {
            auto index = ArrayFindR(children_, [&](auto x){ return x == child; });
            if (!index) {
                break;
            }
            RemoveChildAt(*index);
        }
    }

    void View::RemoveChildAt(int index) {
        auto child = children_[index];
        ArrayRemoveAt(children_, index);
        child->SetParentInternal(nullptr);
    }

    Ptr<Window> View::window() const {
        auto window = window_.lock();
        if (window) {
            return window;
        }
        //  TODO: check
        auto parent = parent_.lock();
        if (parent) {
            return parent->window();
        }
        return nullptr;
    }

    Rect View::frame() const {
        return frame_;
    }

    void View::set_frame(const Rect & value) {
        frame_ = value;

        auto tr = Matrix3x3::RectTransform(Rect(Vector2(0, 0), frame_.size()),
                                           frame_);
        set_transform(tr);
    }

    Color View::background_color() const {
        return background_color_;
    }

    void View::set_background_color(const Color & value) {
        background_color_ = value;
    }

    Matrix3x3 View::transform() const {
        return transform_;
    }

    Matrix3x3 View::window_transform() const {
        UpdateWindowTransform();
        return window_transform_;
    }

    void View::Draw(NVGcontext * ctx) {
        auto window_frame = Rect(Vector2(0, 0), frame_.size())
            .ApplyTransform(window_transform());

        nvgSave(ctx);
        nvgTranslate(ctx,
                     window_frame.origin().x(),
                     window_frame.origin().y());
        nvgScale(ctx,
                 window_frame.size().x() / frame_.size().x(),
                 window_frame.size().y() / frame_.size().y());
        DrawContent(ctx);
        nvgRestore(ctx);

        for (auto & child : children_) {
            child->Draw(ctx);
        }
    }

    void View::DrawContent(NVGcontext * ctx) {
        nvgFillColor(ctx, background_color_.ToNanoVG());
        nvgBeginPath(ctx);
        nvgRect(ctx,
                0,
                0,
                frame_.size().x(),
                frame_.size().y());
        nvgFill(ctx);
    }

    void View::SetWindowInternal(const Ptr<Window> & window) {
        window_ = window;
    }

    void View::SetParentInternal(const Ptr<View> & parent) {
        parent_ = parent;
        SetWindowTransformDirty();
    }

    void View::set_transform(const Matrix3x3 & value) {
        transform_ = value;
        SetWindowTransformDirty();
    }

    void View::SetWindowTransformDirty() {
        window_transform_dirty_ = true;

        for (auto & child : children_) {
            child->SetWindowTransformDirty();
        }
    }

    void View::UpdateWindowTransform() const {
        if (!window_transform_dirty_) {
            return;
        }

        auto parent = parent_.lock();
        if (!parent) {
            window_transform_ = transform_;
            window_transform_dirty_ = false;
            return;
        }

        window_transform_ = transform_ * parent->window_transform();
        window_transform_dirty_ = false;
    }
}
