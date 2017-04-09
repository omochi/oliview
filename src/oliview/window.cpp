#include "./window.h"

#include "./application.h"

namespace oliview {
    Window::Window() {
        window_ = glfwCreateWindow(960, 540, "window", nullptr, nullptr);
        OLIVIEW_ASSERT(window_ != nullptr);

        glfwGetWindowSize(window_, &window_width_, &window_height_);
        glfwGetFramebufferSize(window_, &framebuffer_width_, &framebuffer_height_);
        glfwSetWindowUserPointer(window_, this);
        glfwSetWindowRefreshCallback(window_, &Window::RefreshHandler);
        glfwSetWindowSizeCallback(window_, &Window::WindowSizeHandler);
        glfwSetFramebufferSizeCallback(window_, &Window::FramebufferSizeHandler);

        MakeContextCurrent();

        auto app = Application::shared();
        app->AddWindowInternal(this_ref());
    }

    Window::~Window() {
        if (window_) {
            Close();
        }
    }

    bool Window::closed() const {
        return window_ == nullptr;
    }

    void Window::set_should_close(const std::function<bool()> & value) {
        should_close_ = value;
    }

    GLFWwindow * Window::raw_window() const {
        return window_;
    }

    void Window::Close() {
        glfwDestroyWindow(window_);

        auto app = Application::shared();
        app->RemoveWindowInternal(this_ref());

        glfwSetWindowUserPointer(window_, nullptr);
        window_ = nullptr;
    }

    void Window::Draw() {
        glViewport(0, 0, framebuffer_width_, framebuffer_height_);
        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.5f, -0.5f);
        glVertex2f(0, 0.5f);
        glEnd();
        glfwSwapBuffers(window_);
    }

    void Window::MakeContextCurrent() {
        glfwMakeContextCurrent(window_);
    }

    void Window::MayTryClose() {
        if (glfwWindowShouldClose(window_)) {
            bool do_close = true;
            if (should_close_) {
                do_close = should_close_();
            }
            if (do_close) {
                Close();
            }
        }
    }

    void Window::OnWindowSizeChange(int w, int h) {
        window_width_ = w;
        window_height_ = h;
    }
    void Window::OnFramebufferSizeChange(int w, int h) {
        framebuffer_width_ = w;
        framebuffer_height_ = h;
    }

    void Window::RefreshHandler(GLFWwindow * window) {
        auto thiz = (Window *)glfwGetWindowUserPointer(window);
        thiz->Draw();
    }

    void Window::WindowSizeHandler(GLFWwindow * window, int w, int h) {
        auto thiz = (Window *)glfwGetWindowUserPointer(window);
        thiz->OnWindowSizeChange(w, h);
    }

    void Window::FramebufferSizeHandler(GLFWwindow * window, int w, int h) {
        auto thiz = (Window *)glfwGetWindowUserPointer(window);
        thiz->OnFramebufferSizeChange(w, h);
    }
}
