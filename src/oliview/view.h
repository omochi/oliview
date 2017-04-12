#pragma once

#include "./dependency.h"
#include "./object.h"
#include "./rect.h"
#include "./color.h"
#include "./matrix3x3.h"

namespace oliview {
    class Window;

    class View : public Object<View> {
    public:
        View();

        
        Ref<View> parent() const;
        const std::vector<Ref<View>> & children() const;

        Ref<Window> window() const;

        Rect frame() const;
        void set_frame(const Rect & value);

        Matrix3x3 transform() const;
        Matrix3x3 window_transform() const;

        Color background_color() const;
        void set_background_color(const Color & value);

        void Draw(NVGcontext * ctx);

        void SetWindowInternal(const Ref<Window> & window);
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
