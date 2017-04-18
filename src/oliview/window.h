#pragma once

#include "./dependency.h"
#include "./object.h"
#include "./ref.h"
#include "./view.h"
#include "./gl_assert.h"
#include "./vector2.h"

namespace oliview {
    class Application;

    class Window : public Object<Window> {
    public:
        Window(const Ref<Application> & application);
        virtual ~Window();

        bool closed() const;
        void set_should_close(const std::function<bool()> & value);
        Vector2 window_size() const;
        Vector2 framebuffer_size() const;

        Ref<View> root_view() const;

        void Close();
        void Draw();
        void MakeContextCurrent();
        void MayTryClose();
        void OnWindowSizeChange(int w, int h);
        void OnFramebufferSizeChange(int w, int h);
    private:
        Application * app_;
        
        GLFWwindow * window_;
        NVGcontext * nvg_;

        std::function<bool()> should_close_;

        Vector2 window_size_;
        Vector2 framebuffer_size_;
        Ref<View> root_view_;

        static void RefreshHandler(GLFWwindow * window);
        static void WindowSizeHandler(GLFWwindow * window, int w, int h);
        static void FramebufferSizeHandler(GLFWwindow * window, int w, int h);
    };
}
