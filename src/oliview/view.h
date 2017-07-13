#pragma once

#include "./dependency.h"

#include "./color.h"
#include "./key_event.h"
#include "./layouter.h"
#include "./matrix3x3.h"
#include "./measure_query.h"
#include "./mouse_event.h"
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
        Ptr<View> GetChildAt(size_t index) const;
        size_t child_num() const;
        
        void AddChild(const Ptr<View> & child);
        void InsertChildAt(size_t index, const Ptr<View> & child);
        void RemoveChild(const Ptr<View> & child);
        void RemoveChildAt(size_t index);
        void RemoveFromParent();
        
        size_t GetIndexOfChild(const Ptr<const View> & child) const;
        Ptr<View> GetNextSibling() const;
        Ptr<View> GetPrevSibling() const;

        RHETORIC_GETTER_WEAK(Ptr<Window>, window)        

        RHETORIC_GETTER(Rect, frame)
        void set_frame(const Rect & value);
        
        Rect local_frame() const;

        RHETORIC_GETTER(Color, background_color)
        void set_background_color(const Color & value);
        
        RHETORIC_GETTER(bool, clipping_children)
        void set_clipping_children(bool value);
        
        bool focused() const;
        
        RHETORIC_GETTER(bool, focusable)
        void set_focusable(bool value);
        
        void Focus();
        void Unfocus();
        
        virtual Ptr<View> GetFirstFocusChild() const;
        virtual Ptr<View> GetNextFocusChild(const Ptr<const View> & child) const;
        virtual Ptr<View> GetPrevFocusChild(const Ptr<const View> & child) const;
        virtual Ptr<View> GetLastFocusChild() const;
        
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
        virtual Ptr<View> HitTest(const MouseEvent & event);
        
        virtual void OnMouseDownEvent(const MouseEvent & event);
        virtual void OnMouseMoveEvent(const MouseEvent & event);
        virtual void OnMouseUpEvent(const MouseEvent & event);
        virtual void OnMouseCancelEvent();
        
        virtual bool OnKeyEvent(const KeyEvent & event);
        
        virtual void OnUpdateAnimation(float delta_time);
        
        virtual void OnFocus();
        virtual void OnUnfocus();

        bool _InvokeLayout(NVGcontext * ctx);
        void _PrepareToDraw(const DrawInfo & info);
        void _CollectDrawCommand(std::vector<DrawCommand> * commands, bool shadow);
        void _InvokeDraw(NVGcontext * ctx, bool shadow);
        void _SetParent(const Ptr<View> & parent);
        void _SetWindow(const Ptr<Window> & window);
        MouseEvent _ConvertMouseEventFromWindow(const MouseEvent & event) const;
        void _UpdateAnimation(float delta_time);
    private:
        void Layout(NVGcontext * ctx);
        void DrawBackground(NVGcontext * ctx);
        void DrawShadow(NVGcontext * ctx);
        
        void InvokeWindowOnAddView(const Ptr<Window> & window);
        void InvokeWindowOnRemoveView(const Ptr<Window> & window);
        
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
        bool focusable_;

        DrawInfo draw_info_;
    };
}
