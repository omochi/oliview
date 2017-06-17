#include "./window.h"

#include "./application.h"

namespace oliview {
    Window::Window() {

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


    void Window::Close() {
        if (!window_) {
            return;
        }

        auto app = app_.lock();
        RHETORIC_ASSERT(app != nullptr);
        app->RemoveWindowInternal(shared_from_this());

        root_view_ = nullptr;

        nvgDeleteGL3(nvg_context_);
        nvg_context_ = nullptr;

        glfwDestroyWindow(window_);

        glfwSetWindowUserPointer(window_, nullptr);
        window_ = nullptr;
    }

    Vector2 Window::window_size() const {
        return window_size_;
    }

    Vector2 Window::framebuffer_size() const {
        return framebuffer_size_;
    }

    Ptr<View> Window::root_view() const {
        return root_view_;
    }

    NVGcontext * Window::nvg_context() const {
        return nvg_context_;
    }

    void Window::Draw() {
        OLIVIEW_GL_ASSERT_NO_ERROR();

        View::DrawInfo draw_info;

        root_view_->PreDraw(draw_info);

        if (window_size_.x() > 0 && window_size_.y() > 0) {
            glViewport(0, 0, (int)framebuffer_size_.x(), (int)framebuffer_size_.y());
            OLIVIEW_GL_ASSERT_NO_ERROR();
            glClearColor(0, 0, 0, 0);
            OLIVIEW_GL_ASSERT_NO_ERROR();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            OLIVIEW_GL_ASSERT_NO_ERROR();

            nvgBeginFrame(nvg_context_,
                          (int)window_size_.x(), (int)window_size_.y(),
                          framebuffer_size_.x() / window_size_.x());

            root_view_->set_frame(Rect(Vector2(0, 0), window_size_));
            root_view_->Draw(nvg_context_);
            
            nvgEndFrame(nvg_context_);
            
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

    Ptr<Window> Window::Create(const Ptr<Application> & application) {
        auto thiz = New<Window>();
        thiz->Init(application);
        return thiz;
    }

    void Window::Init(const Ptr<Application> & application) {
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
        RHETORIC_ASSERT(window_ != nullptr);

        app_ = application;
        application->AddWindowInternal(shared_from_this());

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

        nvg_context_ = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);

        root_view_ = New<View>();
        root_view_->SetWindowInternal(shared_from_this());
        
        root_view_->set_background_color(Color(1, 1, 1, 1));
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
