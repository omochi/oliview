#pragma once

#include "./color.h"
#include "./dependency.h"
#include "./matrix3x3.h"
#include "./rect.h"

namespace oliview {
    class Application;
    class Window;

    class View : public std::enable_shared_from_this<View> {
    public:
        struct DrawInfo {
            Matrix3x3 window_transform;

            DrawInfo();
        };

        View();

        Ptr<View> parent() const;
        std::vector<Ptr<View>> children() const;
        void AddChild(const Ptr<View> & child);
        void RemoveChild(const Ptr<View> & child);
        void RemoveChildAt(int index);

        Ptr<Window> window() const;
        Ptr<Application> application() const;

        Rect frame() const;
        void set_frame(const Rect & value);

        Color background_color() const;
        void set_background_color(const Color & value);

        void PreDraw(const DrawInfo & info);
        void Draw(NVGcontext * ctx);

        virtual void DrawContent(NVGcontext * ctx);

        void _SetParent(const Ptr<View> & parent);
        void _SetWindow(const Ptr<Window> & window);
    private:
        WeakPtr<View> parent_;
        std::vector<Ptr<View>> children_;

        WeakPtr<Window> window_;

        Rect frame_;
        Color background_color_;

        DrawInfo draw_info_;
    };
}
