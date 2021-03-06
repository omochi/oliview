#pragma once

#include "./dependency.h"

#include "./application_object.h"
#include "./char_event.h"
#include "./init_macro.h"
#include "./gl_assert.h"
#include "./key_event.h"
#include "./mouse_event.h"
#include "./scroll_event.h"
#include "./size.h"
#include "./view.h"
#include "./vector2.h"
#include "./window_content_view.h"
#include "./window_root_view.h"

namespace oliview {
    class Window : public ApplicationObject {
    public:
        Window();
        virtual ~Window();
        
        virtual void Init(const Ptr<Application> & application);
        
        RHETORIC_SUBCLASS_SHARED_FROM_THIS(Window, ApplicationObject)
        
        bool closed() const;

        void Close();
        
        RHETORIC_GETTER(Size, window_size)
        RHETORIC_GETTER(Size, framebuffer_size)
        RHETORIC_GETTER(float, pixel_ratio)
        Ptr<View> content_view() const;
        RHETORIC_GETTER(GLFWwindow *, glfw_window)
        
        Vector2 mouse_position() const;
        
        bool focused() const;
        
        void Focus();

        void MakeContextCurrent();
        
        virtual bool ShouldClose() const;
        
        RHETORIC_GETTER(Ptr<View>, focused_view)
        
        void FocusNext();
        void FocusPrev();
        void UnfocusView();
        
        virtual void LayoutContentView(NVGcontext * ctx, const Ptr<WindowContentView> & view);
        
        void HandleMouseEvent(const MouseEvent & event);
        void HandleScrollEvent(const ScrollEvent & event);
        void HandleKeyEvent(const KeyEvent & event);
        
        virtual void OnKeyDownEvent(const KeyEvent & event);
        virtual void OnKeyUpEvent(const KeyEvent & event);
        virtual void OnKeyRepeatEvent(const KeyEvent & event);
        
        virtual void Update();
        
        NVGcontext * BeginNVG();
        virtual void Layout(NVGcontext * ctx);
        virtual void Draw(NVGcontext * ctx);
        void EndNVG(NVGcontext * ctx);
        void Swap();
        
        void _InvokeUpdate();
        void _UpdateAnimation(float delta_time);
        void _MayClose();
        
        void _OnAddView(const Ptr<View> & view);
        void _OnRemoveView(const Ptr<View> & view);
        
        void _Focus(const Ptr<View> & view);
    private:
        void CancelNVG(NVGcontext * ctx);
        
        void _Layout(NVGcontext * ctx);
        void _Draw(NVGcontext * ctx);
        
        void set_window_size(const Size & value);
        void set_framebuffer_size(const Size & value);
        
        void MayUpdatePixelRatio();

        Ptr<View> GetNextFocusView(const Ptr<View> & view) const;
        Ptr<View> GetPrevFocusView(const Ptr<View> & view) const;
        
        void PostMouseDownEventTo(const MouseEvent & event, const Ptr<View> & view);
        void BroadcastMouseMoveEventTo(const MouseEvent & window_event, const Ptr<View> & view);
        void CancelMouseEvent();
        
        void PostScrollEventTo(const ScrollEvent & event, const Ptr<View> & view);
        
        GLFWwindow * glfw_window_;
        NVGcontext * nvg_context_;

        std::function<bool()> should_close_;

        Size window_size_;
        Size framebuffer_size_;
        float pixel_ratio_;
        
        Ptr<WindowRootView> root_view_;
        Ptr<WindowContentView> content_view_;
        
        Ptr<View> mouse_target_;
        Option<int> mouse_source_button_;
        
        Ptr<View> focused_view_;
        
        static void RefreshHandler(GLFWwindow * window);
        static void WindowSizeHandler(GLFWwindow * window, int w, int h);
        static void FramebufferSizeHandler(GLFWwindow * window, int w, int h);
        static void FocusHandler(GLFWwindow * window, int focused);
        static void MouseButtonHandler(GLFWwindow * window, int button, int action, int modifier);
        static void CursorPosHandler(GLFWwindow * window, double x, double y);
        static void ScrollHandler(GLFWwindow * window, double x, double y);
        static void KeyHandler(GLFWwindow * window, int key, int scancode, int action, int modifier);
        static void CharHandler(GLFWwindow * window, unsigned int code);
    };
}
