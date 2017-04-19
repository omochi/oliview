#include "./view.h"

#include "./window.h"

namespace oliview {
    View::View():
    parent_(nullptr),
    transform_(Matrix3x3::Identity()),
    window_transform_dirty_(true),
    window_transform_(Matrix3x3::Identity()),
    background_color_(1, 1, 1, 0)
    {
    }

    Ref<View> View::parent() const {
        if (parent_) {
            return Ref<View>(parent_);
        }
        return nullptr;
    }

    const std::vector<Ref<View>> & View::children() const {
        return children_;
    }

    void View::AddChild(const Ref<View> & child) {
        OLIVIEW_ASSERT(child->parent() == nullptr);
        children_.push_back(child);
        child->SetParentInternal(this_ref());
    }

    void View::RemoveChild(const Ref<View> & child) {
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

    Ref<Window> View::window() const {
        if (window_) {
            return Ref<Window>(window_);
        }
        if (parent_) {
            return parent_->window();
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

    void View::SetWindowInternal(const Ref<Window> & window) {
        window_ = window.get();
    }

    void View::SetParentInternal(const Ref<View> & parent) {
        parent_ = parent.get();
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

        if (!parent_) {
            window_transform_ = transform_;
            window_transform_dirty_ = false;
            return;
        }

        window_transform_ = transform_ * parent_->window_transform();
        window_transform_dirty_ = false;
    }
}
