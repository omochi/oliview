#pragma once

#include "./dependency.h"
#include "./rect.h"
#include "./color.h"
#include "./matrix3x3.h"
#include "./array_function.h"

namespace oliview {
    class Window;

    class View : public std::enable_shared_from_this<View> {
    public:
        View();

        Ptr<View> parent() const;
        const std::vector<Ptr<View>> & children() const;
        void AddChild(const Ptr<View> & child);
        void RemoveChild(const Ptr<View> & child);
        void RemoveChildAt(int index);

        Ptr<Window> window() const;

        Rect frame() const;
        void set_frame(const Rect & value);

        Matrix3x3 transform() const;
        Matrix3x3 window_transform() const;

        Color background_color() const;
        void set_background_color(const Color & value);

        void Draw(NVGcontext * ctx);

        virtual void DrawContent(NVGcontext * ctx);

        void SetWindowInternal(const Ptr<Window> & window);
        void SetParentInternal(const Ptr<View> & parent);
    private:
        void set_transform(const Matrix3x3 & value);

        void SetWindowTransformDirty();
        void UpdateWindowTransform() const;

        WeakPtr<View> parent_;
        std::vector<Ptr<View>> children_;
        WeakPtr<Window> window_;
        Rect frame_;
        Matrix3x3 transform_;
        mutable bool window_transform_dirty_;
        mutable Matrix3x3 window_transform_;
        Color background_color_;
    };
}
