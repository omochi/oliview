#pragma once

#include "./dependency.h"
#include "./object.h"
#include "./size.h"
#include "./view.h"

namespace oliview {
    class Window : public Object<Window> {
    public:
        Window();
        virtual ~Window();

        bool closed() const;
        void set_should_close(const std::function<bool()> & value);
        Size window_size() const;
        Size framebuffer_size() const;

        Ref<View> root_view() const;

        void Close();
        void Draw();
        void MakeContextCurrent();
        void MayTryClose();
        void OnWindowSizeChange(int w, int h);
        void OnFramebufferSizeChange(int w, int h);
    private:
        GLFWwindow * window_;
        std::function<bool()> should_close_;

        Size window_size_;
        Size framebuffer_size_;
        Ref<View> root_view_;

        static void RefreshHandler(GLFWwindow * window);
        static void WindowSizeHandler(GLFWwindow * window, int w, int h);
        static void FramebufferSizeHandler(GLFWwindow * window, int w, int h);
    };
}
