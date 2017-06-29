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
            
            Rect clip_frame;

            DrawInfo();
        };

        View(const Ptr<Application> & application);
        virtual ~View();
        
        Ptr<Application> application() const;

        Ptr<View> parent() const;
        std::vector<Ptr<View>> children() const;
        void AddChild(const Ptr<View> & child);
        void RemoveChild(const Ptr<View> & child);
        void RemoveChildAt(int index);

        Ptr<Window> window() const;

        Rect frame() const;
        void set_frame(const Rect & value);

        Color background_color() const;
        void set_background_color(const Color & value);
        
        virtual void Layout();
        bool MayLayout();
        
        void SetNeedsLayout();
        
        RHETORIC_GETTER(std::function<void()>, layout_function)
        void set_layout_function(const std::function<void()> & value);
        
        void PreDraw(const DrawInfo & info);
        void Draw();
        virtual void DrawContent();

        void _SetParent(const Ptr<View> & parent);
        void _SetWindow(const Ptr<Window> & window);
    private:
        WeakPtr<Application> application_;
        
        WeakPtr<View> parent_;
        std::vector<Ptr<View>> children_;

        WeakPtr<Window> window_;

        Rect frame_;
        Color background_color_;
        
        bool needs_layout_;
        
        std::function<void()> layout_function_;

        DrawInfo draw_info_;
    };
}
