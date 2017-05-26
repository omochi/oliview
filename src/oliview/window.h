#pragma once

#include "./dependency.h"
#include "./view.h"
#include "./gl_assert.h"
#include "./vector2.h"

namespace oliview {
    class Application;

    class Window : public std::enable_shared_from_this<Window> {
    public:
        Window();
        virtual ~Window();

        bool closed() const;
        void set_should_close(const std::function<bool()> & value);
        void Close();
        
        Vector2 window_size() const;
        Vector2 framebuffer_size() const;

        Ptr<View> root_view() const;

        NVGcontext * nvg_context() const;



        void Draw();
        void MakeContextCurrent();
        void MayTryClose();
        
        void OnWindowSizeChange(int w, int h);
        void OnFramebufferSizeChange(int w, int h);

        static Ptr<Window> Create(const Ptr<Application> & application);
    private:
        void Init(const Ptr<Application> & application);

        WeakPtr<Application> app_;
        
        GLFWwindow * window_;
        NVGcontext * nvg_context_;

        std::function<bool()> should_close_;

        Vector2 window_size_;
        Vector2 framebuffer_size_;
        Ptr<View> root_view_;

        static void RefreshHandler(GLFWwindow * window);
        static void WindowSizeHandler(GLFWwindow * window, int w, int h);
        static void FramebufferSizeHandler(GLFWwindow * window, int w, int h);
    };
}
