#pragma once

#include "./dependency.h"

#include "./color.h"
#include "./matrix3x3.h"
#include "./nanovg_util.h"
#include "./rect.h"

namespace oliview {
    class Application;
    class Window;

    class View : public std::enable_shared_from_this<View> {
    public:
        struct DrawInfo {
            Matrix3x3 window_transform;
            
            Rect parent_clip_frame;
            Rect content_clip_frame;

            DrawInfo();
        };
        
        struct DrawCommand {
            WeakPtr<View> view;
            bool shadow;
            
            DrawCommand();
        };

        View(const Ptr<Application> & application);
        virtual ~View();
        
        Ptr<Application> application() const;

        Ptr<View> parent() const;
        std::vector<Ptr<View>> children() const;
        
        void AddChild(const Ptr<View> & child);
        void RemoveChild(const Ptr<View> & child);
        void RemoveChildAt(int index);
        void RemoveFromParent();

        Ptr<Window> window() const;

        Rect frame() const;
        void set_frame(const Rect & value);

        Color background_color() const;
        void set_background_color(const Color & value);
        
        bool clip_children() const;
        void set_clip_children(bool value);
        
        virtual void Layout();
        void SetNeedsLayout();
        
        RHETORIC_GETTER(std::function<void()>, layout_function)
        void set_layout_function(const std::function<void()> & value);
        
        virtual void Draw();

        bool _InvokeLayout();
        void _PreDraw(const DrawInfo & info);
        void _CollectDrawCommand(std::list<DrawCommand> * commands);
        void _InvokeDraw(bool shadow);
        void _SetParent(const Ptr<View> & parent);
        void _SetWindow(const Ptr<Window> & window);
    private:
        void LayoutSelf();
        void DrawShadow();
        
        WeakPtr<Application> application_;
        
        WeakPtr<View> parent_;
        std::vector<Ptr<View>> children_;

        WeakPtr<Window> window_;

        Rect frame_;
        Color background_color_;
        
        bool needs_layout_;
        std::function<void()> layout_function_;
        bool self_layouting_;
        bool clip_children_;

        DrawInfo draw_info_;
    };
}
