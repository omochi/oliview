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
    
    void Window::TryClose() {
        if (should_close_) {
            if (!should_close_()) {
                return;
            }
        }
        
        Close();
    }
    
    Ptr<Application> Window::application() const {
        return application_.lock();
    }

    void Window::MakeContextCurrent() {
        glfwMakeContextCurrent(glfw_window_);
    }

    Ptr<Window> Window::Create(const Ptr<Application> & application) {
        auto thiz = RHETORIC_NEW(Window, application);
        thiz->Init();
        return thiz;
    }
    
    void Window::_Update() {
        NVGcontext * ctx = BeginNVG();
        
        Layout(ctx);
        
        Draw(ctx);
        
        EndNVG(ctx);
        
        glfwSwapBuffers(glfw_window_);
    }
    
    void Window::_MayTryClose() {
        if (!glfwWindowShouldClose(glfw_window_)) {
            return;
        }
        
        TryClose();
    }
    
    void Window::_OnWindowSizeChange(int w, int h) {
        set_window_size(Size(w, h));
    }
    
    void Window::_OnFramebufferSizeChange(int w, int h) {
        set_framebuffer_size(Size(w, h));
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
        glfwSetWindowUserPointer(glfw_window_, this);
        glfwSetWindowRefreshCallback(glfw_window_, &Window::RefreshHandler);
        glfwSetWindowSizeCallback(glfw_window_, &Window::WindowSizeHandler);
        glfwSetFramebufferSizeCallback(glfw_window_, &Window::FramebufferSizeHandler);

        MakeContextCurrent();
        
        int w, h;
        
        glfwGetWindowSize(glfw_window_, &w, &h);
        set_window_size(Size(w, h));
        
        glfwGetFramebufferSize(glfw_window_, &w, &h);
        set_framebuffer_size(Size(w, h));
        
        app->_AddWindow(shared_from_this());
    }
    
    NVGcontext * Window::BeginNVG() {
        auto app = application();
        auto ctx = app->_nvg_context();
        
        MakeContextCurrent();
        OLIVIEW_GL_ASSERT_NO_ERROR();
        
        float pixel_ratio = last_valid_framebuffer_size_.width() / last_valid_window_size_.width();
        nvgBeginFrame(ctx,
                      (int)last_valid_window_size_.width(),
                      (int)last_valid_window_size_.height(),
                      pixel_ratio);
        
        return ctx;
    }
    
    void Window::EndNVG(NVGcontext * ctx) {
        nvgEndFrame(ctx);
    }
    
    void Window::Layout(NVGcontext * ctx) {
        int layout_count = 0;
        while (true) {
            bool updated = root_view_->_InvokeLayout(ctx);
            layout_count += 1;
            if (!updated) {
                break;
            }
            
            RHETORIC_ASSERT(layout_count < 100);
        }
    }
    
    void Window::Draw(NVGcontext * ctx) {
        if (framebuffer_size_.width() == 0 ||
            framebuffer_size_.height() == 0)
        {
            return;
        }
        
        View::DrawInfo draw_info;
        draw_info.parent_clip_frame = Rect(Vector2(0, 0), window_size_);
        root_view_->_PrepareToDraw(draw_info);
        
        std::list<View::DrawCommand> draws;
        {
            View::DrawCommand command;
            command.view = root_view_;
            command.shadow = false;
            draws.push_back(command);
        }
        root_view_->_CollectDrawCommand(&draws);
        
        glViewport(0, 0, (int)framebuffer_size_.width(), (int)framebuffer_size_.height());
        OLIVIEW_GL_ASSERT_NO_ERROR();
        glClearColor(0, 0, 0, 0);
        OLIVIEW_GL_ASSERT_NO_ERROR();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        OLIVIEW_GL_ASSERT_NO_ERROR();
        
        for (auto & draw : draws) {
            auto view = draw.view.lock();
            if (!view) { continue; }
            view->_InvokeDraw(ctx, draw.shadow);
        }
    }
    
    void Window::set_window_size(const Size & value) {
        window_size_ = value;
        
        if (window_size_.width() != 0 && window_size_.height() != 0) {
            last_valid_window_size_ = window_size_;
        }
        
        root_view_->set_frame(Rect(Vector2(0, 0), window_size_));
    }
    
    void Window::set_framebuffer_size(const Size & value) {
        framebuffer_size_ = value;
        
        if (framebuffer_size_.width() != 0 && framebuffer_size_.height() != 0) {
            last_valid_framebuffer_size_ = framebuffer_size_;
        }
    }

    void Window::RefreshHandler(GLFWwindow * window) {
        auto thiz = (Window *)glfwGetWindowUserPointer(window);
        thiz->_Update();
    }

    void Window::WindowSizeHandler(GLFWwindow * window, int w, int h) {
        auto thiz = (Window *)glfwGetWindowUserPointer(window);
        thiz->_OnWindowSizeChange(w, h);
    }

    void Window::FramebufferSizeHandler(GLFWwindow * window, int w, int h) {
        auto thiz = (Window *)glfwGetWindowUserPointer(window);
        thiz->_OnFramebufferSizeChange(w, h);
    }
}
