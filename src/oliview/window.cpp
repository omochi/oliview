#include "./window.h"

#include "./application.h"

namespace oliview {

    Window::~Window() {
        Close();
    }

    bool Window::closed() const {
        return glfw_window_ == nullptr;
    }
    
    void Window::Close() {
        if (!glfw_window_) {
            return;
        }

        auto app = application();
        RHETORIC_ASSERT(app != nullptr);
        app->_RemoveWindow(shared_from_this());

        root_view_ = nullptr;

        glfwDestroyWindow(glfw_window_);

        glfwSetWindowUserPointer(glfw_window_, nullptr);
        glfw_window_ = nullptr;
    }
    
    Ptr<Application> Window::application() const {
        return application_.lock();
    }

    void Window::Draw() {
        OLIVIEW_GL_ASSERT_NO_ERROR();

        View::DrawInfo draw_info;

        draw_info.clip_frame = Rect(Vector2(0, 0),
                                    window_size_);
        root_view_->PreDraw(draw_info);

        if (window_size_.width() > 0 && window_size_.height() > 0) {
            MakeContextCurrent();
            
            glViewport(0, 0, (int)framebuffer_size_.width(), (int)framebuffer_size_.height());
            OLIVIEW_GL_ASSERT_NO_ERROR();
            glClearColor(0, 0, 0, 0);
            OLIVIEW_GL_ASSERT_NO_ERROR();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            OLIVIEW_GL_ASSERT_NO_ERROR();

            auto app = application();
            auto ctx = app->nvg_context();
            
            nvgBeginFrame(ctx,
                          (int)window_size_.width(),
                          (int)window_size_.height(),
                          framebuffer_size_.width() / window_size_.width());

            root_view_->set_frame(Rect(Vector2(0, 0), window_size_));
            root_view_->Draw(ctx);
            
            nvgEndFrame(ctx);
            
            glfwSwapBuffers(glfw_window_);
        }
    }

    void Window::MakeContextCurrent() {
        glfwMakeContextCurrent(glfw_window_);
    }

    void Window::MayTryClose() {
        if (glfwWindowShouldClose(glfw_window_)) {
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

    Ptr<Window> Window::Create(const Ptr<Application> & application) {
        auto thiz = RHETORIC_NEW(Window, application);
        thiz->Init();
        return thiz;
    }

    Window::Window(const Ptr<Application> & application):
    application_(application),
    glfw_window_(nullptr)
    {
    }
    
    void Window::Init() {
        auto app = application();

        root_view_ = New<View>(app);
        root_view_->_SetWindow(shared_from_this());
        
        root_view_->set_background_color(Color(1, 1, 1, 1));
        
        Open();
    }
    
    void Window::Open() {
        auto app = application();
        
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
        
        GLFWwindow * gfscw = nullptr;
        auto scw = app->_shared_context_window();
        if (scw) {
            gfscw = scw->glfw_window();
        }
        
        glfw_window_ = glfwCreateWindow(960, 540, "window", nullptr, gfscw);
        RHETORIC_ASSERT(glfw_window_ != nullptr);
        
        int w, h;
        
        glfwSetWindowUserPointer(glfw_window_, this);
        
        glfwSetWindowRefreshCallback(glfw_window_, &Window::RefreshHandler);
        
        glfwGetWindowSize(glfw_window_, &w, &h);
        window_size_ = Size(w, h);
        glfwSetWindowSizeCallback(glfw_window_, &Window::WindowSizeHandler);
        
        glfwGetFramebufferSize(glfw_window_, &w, &h);
        framebuffer_size_ = Size(w, h);
        glfwSetFramebufferSizeCallback(glfw_window_, &Window::FramebufferSizeHandler);
        
        MakeContextCurrent();
        
        app->_AddWindow(shared_from_this());
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
