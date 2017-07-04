#pragma once

#include "./dependency.h"

#include "./color.h"
#include "./layouter.h"
#include "./matrix3x3.h"
#include "./measure_query.h"
#include "./nanovg_util.h"
#include "./rect.h"
#include "./size.h"

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

        RHETORIC_GETTER(Rect, frame)
        void set_frame(const Rect & value);

        RHETORIC_GETTER(Color, background_color)
        void set_background_color(const Color & value);
        
        RHETORIC_GETTER(bool, clipping_children)
        void set_clipping_children(bool value);
        
        void SetNeedsLayout();
        
        
        RHETORIC_GETTER(Ptr<Layouter>, layouter)
        void set_layouter(const Ptr<Layouter> & value);
        
        virtual void OnLayout();
        
        virtual Size Measure(const MeasureQuery & query) const;
        virtual Size OnMeasure(const MeasureQuery & query) const;
        
        virtual void Draw();

        bool _InvokeLayout();
        void _PreDraw(const DrawInfo & info);
        void _CollectDrawCommand(std::list<DrawCommand> * commands);
        void _InvokeDraw(bool shadow);
        void _SetParent(const Ptr<View> & parent);
        void _SetWindow(const Ptr<Window> & window);
    private:
        void Layout();
        void DrawShadow();
        
        WeakPtr<Application> application_;
        
        WeakPtr<View> parent_;
        std::vector<Ptr<View>> children_;

        WeakPtr<Window> window_;

        Rect frame_;
        Color background_color_;
        
        bool needs_layout_;
        Ptr<Layouter> layouter_;
        
        bool self_layouting_;
        bool clipping_children_;

        DrawInfo draw_info_;
    };
}
