#pragma once

#include "./dependency.h"
#include "./object.h"
#include "./ref.h"
#include "./rect.h"
#include "./color.h"
#include "./matrix3x3.h"
#include "./array_function.h"

namespace oliview {
    class Window;

    class View : public Object<View> {
    public:
        View();

        Ref<View> parent() const;
        const std::vector<Ref<View>> & children() const;
        void AddChild(const Ref<View> & child);
        void RemoveChild(const Ref<View> & child);
        void RemoveChildAt(int index);

        Ref<Window> window() const;

        Rect frame() const;
        void set_frame(const Rect & value);

        Matrix3x3 transform() const;
        Matrix3x3 window_transform() const;

        Color background_color() const;
        void set_background_color(const Color & value);

        void Draw(NVGcontext * ctx);

        virtual void DrawContent(NVGcontext * ctx);

        void SetWindowInternal(const Ref<Window> & window);
        void SetParentInternal(const Ref<View> & parent);
    private:
        void set_transform(const Matrix3x3 & value);

        void SetWindowTransformDirty();
        void UpdateWindowTransform() const;

        View * parent_;
        std::vector<Ref<View>> children_;
        Window * window_;
        Rect frame_;
        Matrix3x3 transform_;
        mutable bool window_transform_dirty_;
        mutable Matrix3x3 window_transform_;
        Color background_color_;
    };
}
