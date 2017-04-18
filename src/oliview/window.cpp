#include "./window.h"

#include "./application.h"

namespace oliview {
    Window::Window(const Ref<Application> & application) {
        Ref<Window> this_ref(this);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RED_BITS, 8);
        glfwWindowHint(GLFW_GREEN_BITS, 8);
        glfwWindowHint(GLFW_BLUE_BITS, 8);
        glfwWindowHint(GLFW_ALPHA_BITS, 8);
        glfwWindowHint(GLFW_DEPTH_BITS, 24);
        glfwWindowHint(GLFW_STENCIL_BITS, 8);
        
        window_ = glfwCreateWindow(960, 540, "window", nullptr, nullptr);
        OLIVIEW_ASSERT(window_ != nullptr);

        app_ = application.get();
        app_->AddWindowInternal(this_ref);

        int w, h;

        glfwSetWindowUserPointer(window_, this);

        glfwSetWindowRefreshCallback(window_, &Window::RefreshHandler);

        glfwGetWindowSize(window_, &w, &h);
        window_size_ = Vector2(w, h);
        glfwSetWindowSizeCallback(window_, &Window::WindowSizeHandler);

        glfwGetFramebufferSize(window_, &w, &h);
        framebuffer_size_ = Vector2(w, h);
        glfwSetFramebufferSizeCallback(window_, &Window::FramebufferSizeHandler);

        MakeContextCurrent();

        nvg_ = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);

        root_view_ = Create<View>();
        root_view_->SetWindowInternal(this_ref);
    }

    Window::~Window() {
        Close();
    }

    bool Window::closed() const {
        return window_ == nullptr;
    }

    void Window::set_should_close(const std::function<bool()> & value) {
        should_close_ = value;
    }

    Vector2 Window::window_size() const {
        return window_size_;
    }

    Vector2 Window::framebuffer_size() const {
        return framebuffer_size_;
    }

    Ref<View> Window::root_view() const {
        return root_view_;
    }

    void Window::Close() {
        if (!window_) {
            return;
        }

        Ref<Window> this_ref(this);

        app_->RemoveWindowInternal(this_ref);

        root_view_ = nullptr;

        nvgDeleteGL3(nvg_);
        nvg_ = nullptr;

        glfwDestroyWindow(window_);

        glfwSetWindowUserPointer(window_, nullptr);
        window_ = nullptr;
    }

    void Window::Draw() {
        OLIVIEW_GL_ASSERT_NO_ERROR();

        if (window_size_.x() > 0 && window_size_.y() > 0) {
            glViewport(0, 0, (int)framebuffer_size_.x(), (int)framebuffer_size_.y());
            OLIVIEW_GL_ASSERT_NO_ERROR();
            glClearColor(0, 0, 0, 0);
            OLIVIEW_GL_ASSERT_NO_ERROR();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            OLIVIEW_GL_ASSERT_NO_ERROR();

            nvgBeginFrame(nvg_, (int)window_size_.x(), (int)window_size_.y(),
                          framebuffer_size_.x() / window_size_.x());

            root_view_->set_frame(Rect(Vector2(0, 0), window_size_));
            root_view_->Draw(nvg_);
            
            nvgEndFrame(nvg_);
            
            glfwSwapBuffers(window_);
        }
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
        window_size_ = Vector2(w, h);
    }

    void Window::OnFramebufferSizeChange(int w, int h) {
        framebuffer_size_ = Vector2(w, h);
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
