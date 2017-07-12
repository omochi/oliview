#pragma once

#include "./dependency.h"

#include "./init_macro.h"
#include "./gl_assert.h"
#include "./mouse_event.h"
#include "./size.h"
#include "./view.h"
#include "./vector2.h"

namespace oliview {
    class Window :
    public std::enable_shared_from_this<Window> {
    public:
        Window();
        virtual ~Window();
        
        virtual void Init(const Ptr<Application> & application);

        RHETORIC_GETTER_WEAK(Ptr<Application>, application)
        
        bool closed() const;

        void Close();
        
        RHETORIC_GETTER(Size, window_size)
        RHETORIC_GETTER(Size, framebuffer_size)
        RHETORIC_GETTER(Ptr<View>, root_view)
        RHETORIC_GETTER(GLFWwindow *, glfw_window)

        void MakeContextCurrent();
        
        virtual bool ShouldClose() const;
        
        void RefreshLayout();
        
        RHETORIC_GETTER(Ptr<View>, focused_view)
        
        void FocusNext();
        void FocusPrev();
        
        void HandleMouseEvent(const MouseEvent & event);
        
        void _Update();
        void _UpdateAnimation(float delta_time);
        void _MayClose();
        
        void _OnAddView(const Ptr<View> & view);
        void _OnRemoveView(const Ptr<View> & view);
        
        void _Focus(const Ptr<View> & view);
    private:        
        NVGcontext * BeginNVG();
        void EndNVG(NVGcontext * ctx);
        void CancelNVG(NVGcontext * ctx);
        
        void Layout(NVGcontext * ctx);
        void Draw(NVGcontext * ctx);
        
        void set_window_size(const Size & value);
        void set_framebuffer_size(const Size & value);

        WeakPtr<Application> application_;
        GLFWwindow * glfw_window_;
        NVGcontext * nvg_context_;

        std::function<bool()> should_close_;

        Size window_size_;
        Size framebuffer_size_;
        
        Size last_valid_window_size_;
        Size last_valid_framebuffer_size_;
        
        Ptr<View> root_view_;
        
        Ptr<View> mouse_target_;
        Optional<int> mouse_source_button_;
        
        Ptr<View> focused_view_;
        
        static void RefreshHandler(GLFWwindow * window);
        static void WindowSizeHandler(GLFWwindow * window, int w, int h);
        static void FramebufferSizeHandler(GLFWwindow * window, int w, int h);
        static void MouseButtonHandler(GLFWwindow * window, int button, int action, int modifier);
        static void CursorPosHandler(GLFWwindow * window, double x, double y);
        static void CharHandler(GLFWwindow * window, unsigned int code);
    };
}
