#pragma once

#include "./dependency.h"

#include "./application_object.h"
#include "./char_event.h"
#include "./color.h"
#include "./edge_inset.h"
#include "./key_event.h"
#include "./layouter.h"
#include "./matrix3x3.h"
#include "./measure_query.h"
#include "./mouse_event.h"
#include "./nanovg_util.h"
#include "./rect.h"
#include "./scroll_event.h"
#include "./size.h"

namespace oliview {
    class Application;
    class Window;
    
    /*
     Measure: MeasureOwnContentとLayouterの結果をMaxして返す。
     MeasureOwnContent: 継承を想定。
     Layout: LayoutOwnContentとLayouterのLayoutを呼び出す。
     LayoutOwnContent: 継承を想定。
     DrawOwnContent: 継承を想定。
     */

    class View : public ApplicationObject {
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
        
        RHETORIC_SUBCLASS_SHARED_FROM_THIS(View, ApplicationObject)
        
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
        
        RHETORIC_GETTER(Rect, bounds)
        void set_bounds(const Rect & value);
        
        RHETORIC_GETTER(Color, background_color)
        void set_background_color(const Color & value);
        
        RHETORIC_GETTER(bool, clipping_children)
        void set_clipping_children(bool value);
        
        RHETORIC_GETTER(bool, focused)
        
        RHETORIC_GETTER(bool, visible);
        void set_visible(bool value);
        
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
        
        virtual Size Measure(NVGcontext * ctx, const MeasureQuery & query) const;
        virtual Size MeasureOwnContent(NVGcontext * ctx, const MeasureQuery & query) const;

        virtual void Layout(NVGcontext * ctx);
        virtual void LayoutOwnContent(NVGcontext * ctx);
    
        virtual void DrawOwnContent(NVGcontext * ctx);
        
        Vector2 ConvertPointToWindow(const Vector2 & point) const;
        Vector2 ConvertPointFromWindow(const Vector2 & point) const;
        Vector2 ConvertPointToView(const Vector2 & point, const Ptr<View> & view) const;
        Vector2 ConvertPointFromView(const Vector2 & point, const Ptr<View> & view) const;

        virtual bool IsPointInside(const Vector2 & point) const;
        virtual Ptr<View> MouseHitTest(const Vector2 & pos) const;
        
        virtual bool OnMouseDownEvent(const MouseEvent & event);
        virtual void OnMouseMoveEvent(const MouseEvent & event);
        virtual void OnMouseUpEvent(const MouseEvent & event);
        virtual void OnMouseCancelEvent();
        
        virtual bool OnScrollEvent(const ScrollEvent & event);
        
        virtual bool OnKeyDownEvent(const KeyEvent & event);
        virtual bool OnKeyUpEvent(const KeyEvent & event);
        virtual bool OnKeyRepeatEvent(const KeyEvent & event);
        
        virtual void OnCharEvent(const CharEvent & event);
        
        virtual void OnUpdateAnimation(float delta_time);
        
        virtual void OnFocus();
        virtual void OnUnfocus();

        bool _InvokeLayout(NVGcontext * ctx);
        void _PrepareToDraw(const DrawInfo & info);
        void _CollectDrawCommand(std::vector<DrawCommand> * commands);
        void _InvokeDraw(NVGcontext * ctx, bool shadow);
        void _SetParent(const Ptr<View> & parent);
        void _SetWindow(const Ptr<Window> & window);
        MouseEvent _ConvertMouseEventFromWindow(const MouseEvent & event) const;
        ScrollEvent _ConvertScrollEventFromWindow(const ScrollEvent & event) const;
        void _SetFocused(bool value);
        void _UpdateAnimation(float delta_time);
    private:
        void DrawBackground(NVGcontext * ctx);
        void DrawShadow(NVGcontext * ctx);
        
        void InvokeWindowOnAddView(const Ptr<Window> & window);
        void InvokeWindowOnRemoveView(const Ptr<Window> & window);
        
        WeakPtr<View> parent_;
        std::vector<Ptr<View>> children_;

        WeakPtr<Window> window_;

        Rect frame_;
        Rect bounds_;
        Color background_color_;
        
        bool needs_layout_;
        Ptr<Layouter> children_layouter_;
        
        bool clipping_children_;
        bool focusable_;
        
        bool visible_;
        bool focused_;

        DrawInfo draw_info_;
    };
}
