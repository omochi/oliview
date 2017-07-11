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

    class View :
    public std::enable_shared_from_this<View> {
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

        View();
        virtual ~View();
        virtual void Init(const Ptr<Application> & application);
        
        RHETORIC_GETTER_WEAK(Ptr<Application>, application)
        RHETORIC_GETTER_WEAK(Ptr<View>, parent)
        RHETORIC_GETTER(std::vector<Ptr<View>>, children)
        
        void AddChild(const Ptr<View> & child);
        void RemoveChild(const Ptr<View> & child);
        void RemoveChildAt(size_t index);
        void RemoveFromParent();

        RHETORIC_GETTER_WEAK(Ptr<Window>, window)        

        RHETORIC_GETTER(Rect, frame)
        void set_frame(const Rect & value);
        
        Rect local_frame() const;

        RHETORIC_GETTER(Color, background_color)
        void set_background_color(const Color & value);
        
        RHETORIC_GETTER(bool, clipping_children)
        void set_clipping_children(bool value);
        
        void SetNeedsLayout();
        
        RHETORIC_GETTER(Ptr<Layouter>, children_layouter)
        void set_children_layouter(const Ptr<Layouter> & value);
        
        Size Measure(NVGcontext * ctx, const MeasureQuery & query) const;
        virtual Size MeasureContent(NVGcontext * ctx, const MeasureQuery & query) const;

        virtual void LayoutContent(NVGcontext * ctx);
    
        virtual void DrawContent(NVGcontext * ctx);
        
        Matrix3x3 local_transform() const;
        Matrix3x3 window_transform() const;
        
        Vector2 ConvertPointToWindow(const Vector2 & point) const;
        Vector2 ConvertPointFromWindow(const Vector2 & point) const;
        Vector2 ConvertPointToView(const Vector2 & point, const Ptr<View> & view) const;
        Vector2 ConvertPointFromView(const Vector2 & point, const Ptr<View> & view) const;

        virtual bool IsPointInside(const Vector2 & point) const;
        virtual Ptr<View> HitTest(const Vector2 & point);

        bool _InvokeLayout(NVGcontext * ctx);
        void _PrepareToDraw(const DrawInfo & info);
        void _CollectDrawCommand(std::vector<DrawCommand> * commands, bool shadow);
        void _InvokeDraw(NVGcontext * ctx, bool shadow);
        void _SetParent(const Ptr<View> & parent);
        void _SetWindow(const Ptr<Window> & window);
    private:
        void Layout(NVGcontext * ctx);
        void DrawBackground(NVGcontext * ctx);
        void DrawShadow(NVGcontext * ctx);
        
        WeakPtr<Application> application_;
        WeakPtr<View> parent_;
        std::vector<Ptr<View>> children_;

        WeakPtr<Window> window_;

        Rect frame_;
        Color background_color_;
        
        bool needs_layout_;
        Ptr<Layouter> children_layouter_;
        
        bool self_layouting_;
        bool clipping_children_;

        DrawInfo draw_info_;
    };
}
