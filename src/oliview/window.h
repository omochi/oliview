#pragma once

#include "./dependency.h"
#include "./object.h"

namespace oliview {
    class Window : public Object<Window> {
    public:
        Window();
        virtual ~Window();

        bool closed() const;
        void set_should_close(const std::function<bool()> & value);

        GLFWwindow * raw_window() const;

        void Close();
        void Draw();
        void MakeContextCurrent();
        void MayTryClose();
        void OnWindowSizeChange(int w, int h);
        void OnFramebufferSizeChange(int w, int h);
    private:
        GLFWwindow * window_;
        std::function<bool()> should_close_;
        int window_width_;
        int window_height_;
        int framebuffer_width_;
        int framebuffer_height_;

        static void RefreshHandler(GLFWwindow * window);
        static void WindowSizeHandler(GLFWwindow * window, int w, int h);
        static void FramebufferSizeHandler(GLFWwindow * window, int w, int h);
    };
}
