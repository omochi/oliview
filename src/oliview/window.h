#pragma once

#include "./dependency.h"
#include "./view.h"
#include "./gl_assert.h"
#include "./vector2.h"

namespace oliview {
    class Application;

    class Window : public std::enable_shared_from_this<Window> {
    public:
        virtual ~Window();

        bool closed() const;
        
        RHETORIC_ACCESSOR_TRIVIAL(std::function<bool()>, should_close)

        void Close();
        
        Ptr<Application> application() const;
        RHETORIC_GETTER(Vector2, window_size)
        RHETORIC_GETTER(Vector2, framebuffer_size)
        RHETORIC_GETTER(Ptr<View>, root_view)
        RHETORIC_GETTER(GLFWwindow *, glfw_window)

        void Draw();
        void MakeContextCurrent();
        void MayTryClose();
        
        void OnWindowSizeChange(int w, int h);
        void OnFramebufferSizeChange(int w, int h);

        static Ptr<Window> Create(const Ptr<Application> & application);
    private:
        Window(const Ptr<Application> & application);
        void Init();
        void Open();

        WeakPtr<Application> application_;
        
        GLFWwindow * glfw_window_;

        std::function<bool()> should_close_;

        Vector2 window_size_;
        Vector2 framebuffer_size_;
        Ptr<View> root_view_;

        static void RefreshHandler(GLFWwindow * window);
        static void WindowSizeHandler(GLFWwindow * window, int w, int h);
        static void FramebufferSizeHandler(GLFWwindow * window, int w, int h);
    };
}
