#include "./window.h"

#include "./application.h"

namespace oliview {
    Window::Window():
    glfw_window_(nullptr),
    nvg_context_(nullptr)
    {}

    Window::~Window() {
        Close();
    }

    void Window::Init(const Ptr<Application> & app) {
        application_ = app;
        
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
        
        GLFWwindow * gsw = nullptr;
        auto sw = app->_shared_context_window();
        if (sw) {
            gsw = sw->glfw_window();
        }
        
        GLFWwindow * gw = glfwCreateWindow(960, 540, "window", nullptr, gsw);
        RHETORIC_ASSERT(gw != nullptr);
        glfw_window_ = gw;
        
        glfwSetWindowUserPointer(gw, this);
        glfwSetWindowRefreshCallback(gw, &Window::RefreshHandler);
        glfwSetWindowSizeCallback(gw, &Window::WindowSizeHandler);
        glfwSetFramebufferSizeCallback(gw, &Window::FramebufferSizeHandler);
        glfwSetMouseButtonCallback(gw, &Window::MouseButtonHandler);
        glfwSetCursorPosCallback(gw, &Window::CursorPosHandler);
        glfwSetCharCallback(gw, &Window::CharHandler);
        
        MakeContextCurrent();
        
        root_view_ = OLIVIEW_INIT(View, app);
        root_view_->_SetWindow(shared_from_this());
        root_view_->set_background_color(Color(1, 1, 1, 1));
        
        int w, h;
        
        glfwGetWindowSize(gw, &w, &h);
        set_window_size(Size(w, h));
        
        glfwGetFramebufferSize(gw, &w, &h);
        set_framebuffer_size(Size(w, h));
        
        app->_AddWindow(shared_from_this());
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
    
    void Window::MakeContextCurrent() {
        glfwMakeContextCurrent(glfw_window_);
    }
    
    bool Window::ShouldClose() const {
        return true;
    }
    
    void Window::RefreshLayout() {
        if (nvg_context_) {
            Layout(nvg_context_);
        } else {
            NVGcontext * ctx = BeginNVG();
            Layout(ctx);
            CancelNVG(ctx);
        }
    }
    
    void Window::HandleMouseEvent(const MouseEvent & event_) {
        MouseEvent event = event_;
        
        if (mouse_target_) {
            event = mouse_target_->_ConvertMouseEventFromWindow(event);
            
            if (event.type() == MouseEventType::Up) {
                RHETORIC_ASSERT(event.button().presented());
                
                if (mouse_source_button_ == event.button()) {
                    mouse_target_->OnMouseUpEvent(event);
                    
                    mouse_target_ = nullptr;
                    mouse_source_button_ = None();
                }
            } else if (event.type() == MouseEventType::Move) {
                mouse_target_->OnMouseMoveEvent(event);
            }
        } else {
            if (event.type() == MouseEventType::Down) {
                RHETORIC_ASSERT(event.button().presented());
                
                Ptr<View> hit_target = root_view_->HitTest(event);
                if (hit_target) {
                    event = hit_target->_ConvertMouseEventFromWindow(event);
                    
                    mouse_target_ = hit_target;
                    mouse_source_button_ = event.button();
                    
                    hit_target->OnMouseDownEvent(event);
                }
            }
        }
        
        RefreshLayout();
    }
    
    void Window::_Update() {
        NVGcontext * ctx = BeginNVG();
        Layout(ctx);
        Draw(ctx);
        EndNVG(ctx);
        glfwSwapBuffers(glfw_window_);
        
        RefreshLayout();
    }
    
    void Window::_MayClose() {
        if (!glfwWindowShouldClose(glfw_window_)) {
            return;
        }
        
        if (!ShouldClose()) {
            return;
        }
        
        Close();
    }
    
    void Window::_OnAddView(const Ptr<View> & view) {
        RHETORIC_UNUSED(view);
    }
    
    void Window::_OnRemoveView(const Ptr<View> & view) {
        if (view == mouse_target_) {
            mouse_target_->OnMouseCancelEvent();
            
            mouse_target_ = nullptr;
            mouse_source_button_ = None();
        }
    }
    
    NVGcontext * Window::BeginNVG() {
        RHETORIC_ASSERT(nvg_context_ == nullptr);
        
        auto app = application();
        auto ctx = app->_nvg_context();
        
        MakeContextCurrent();
        OLIVIEW_GL_ASSERT_NO_ERROR();
        
        float pixel_ratio = last_valid_framebuffer_size_.width() / last_valid_window_size_.width();
        nvgBeginFrame(ctx,
                      (int)last_valid_window_size_.width(),
                      (int)last_valid_window_size_.height(),
                      pixel_ratio);
        
        nvg_context_ = ctx;
        
        return ctx;
    }
    
    void Window::EndNVG(NVGcontext * ctx) {
        RHETORIC_ASSERT(nvg_context_ == ctx);
        
        nvgEndFrame(ctx);
        nvg_context_ = nullptr;
    }
    
    void Window::CancelNVG(NVGcontext * ctx) {
        RHETORIC_ASSERT(nvg_context_ == ctx);

        nvgCancelFrame(ctx);
        nvg_context_ = nullptr;
    }
    
    void Window::Layout(NVGcontext * ctx) {
        int count = 0;
        while (true) {
            bool updated = root_view_->_InvokeLayout(ctx);
            count += 1;
            if (!updated) {
                break;
            }
            
            RHETORIC_ASSERT(count < 100);
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

        std::vector<View::DrawCommand> draws;
        {
            View::DrawCommand command;
            command.view = root_view_;
            command.shadow = false;
            draws.push_back(command);
        }
        root_view_->_CollectDrawCommand(&draws, true);
        
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
        thiz->set_window_size(Size(w, h));
    }

    void Window::FramebufferSizeHandler(GLFWwindow * window, int w, int h) {
        auto thiz = (Window *)glfwGetWindowUserPointer(window);
        thiz->set_framebuffer_size(Size(w, h));
    }
    
    void Window::MouseButtonHandler(GLFWwindow * window, int button, int action, int modifier) {
        auto thiz = (Window *)glfwGetWindowUserPointer(window);
        
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        
        MouseEvent event;
        if (action == GLFW_PRESS) {
            event.set_type(MouseEventType::Down);
        } else if (action == GLFW_RELEASE) {
            event.set_type(MouseEventType::Up);
        } else {
            return;
        }
        event.set_pos(Vector2((float)x, (float)y));
        event.set_button(Some(button));
        event.set_modifier(Some(modifier));

        thiz->HandleMouseEvent(event);
    }
    
    void Window::CursorPosHandler(GLFWwindow * window, double x, double y) {
        auto thiz = (Window *)glfwGetWindowUserPointer(window);
        
        MouseEvent event;
        event.set_type(MouseEventType::Move);
        event.set_pos(Vector2((float)x, (float)y));

        thiz->HandleMouseEvent(event);
    }
    
    void Window::CharHandler(GLFWwindow * window, unsigned int code) {
        RHETORIC_UNUSED(window);
        // TODO; test
        Print(Format("0x%04x", code));
    }
}
