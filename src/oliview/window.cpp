#include "./window.h"

#include "./application.h"

namespace oliview {
    Window::Window() {
        window_ = glfwCreateWindow(960, 540, "window", nullptr, nullptr);
        OLIVIEW_ASSERT(window_ != nullptr);

        auto app = Application::shared();
        app->AddWindowInternal(this_ref());

        int w, h;

        glfwSetWindowUserPointer(window_, this);

        glfwSetWindowRefreshCallback(window_, &Window::RefreshHandler);

        glfwGetWindowSize(window_, &w, &h);
        window_size_ = Size(w, h);
        glfwSetWindowSizeCallback(window_, &Window::WindowSizeHandler);

        glfwGetFramebufferSize(window_, &w, &h);
        framebuffer_size_ = Size(w, h);
        glfwSetFramebufferSizeCallback(window_, &Window::FramebufferSizeHandler);

        MakeContextCurrent();
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

    Size Window::window_size() const {
        return window_size_;
    }

    Size Window::framebuffer_size() const {
        return framebuffer_size_;
    }

    Ref<View> Window::root_view() const {
        return root_view_;
    }

    void Window::Close() {
        auto app = Application::shared();
        app->RemoveWindowInternal(this_ref());

        glfwDestroyWindow(window_);

        glfwSetWindowUserPointer(window_, nullptr);
        window_ = nullptr;
    }

    void Window::Draw() {
        glViewport(0, 0, framebuffer_size_.width(), framebuffer_size_.height());
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
        window_size_ = Size(w, h);
    }

    void Window::OnFramebufferSizeChange(int w, int h) {
        framebuffer_size_ = Size(w, h);
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
