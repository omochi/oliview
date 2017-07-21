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
        ApplicationObject::Init(app);
        
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
        glfwSetWindowFocusCallback(gw, &Window::FocusHandler);
        glfwSetMouseButtonCallback(gw, &Window::MouseButtonHandler);
        glfwSetCursorPosCallback(gw, &Window::CursorPosHandler);
        glfwSetScrollCallback(gw, &Window::ScrollHandler);
        glfwSetKeyCallback(gw, &Window::KeyHandler);
        glfwSetCharCallback(gw, &Window::CharHandler);
        
        MakeContextCurrent();
        
        int w, h;
        
        glfwGetWindowSize(gw, &w, &h);
        set_window_size(Size(w, h));
        
        glfwGetFramebufferSize(gw, &w, &h);
        set_framebuffer_size(Size(w, h));
        
        root_view_ = OLIVIEW_INIT(WindowRootView, app);
        root_view_->_SetWindow(shared_from_this());
        root_view_->set_background_color(Color(1, 1, 1, 1));
        root_view_->set_frame(Rect(Vector2(), window_size()));
        
        content_view_ = OLIVIEW_INIT(WindowContentView, app);
        root_view_->AddChild(content_view_);
        content_view_->set_background_color(Color(1, 1, 1, 1));
        content_view_->set_frame(root_view_->bounds());
                
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

        glfwDestroyWindow(glfw_window_);

        glfwSetWindowUserPointer(glfw_window_, nullptr);
        glfw_window_ = nullptr;
    }
    
    Ptr<View> Window::content_view() const {
        return content_view_;
    }
    
    bool Window::focused() const {
        return glfwGetWindowAttrib(glfw_window_, GLFW_FOCUSED) != 0;
    }
    
    void Window::Focus() {
        glfwFocusWindow(glfw_window_);
    }
    
    void Window::MakeContextCurrent() {
        if (closed()) { return; }
        glfwMakeContextCurrent(glfw_window_);
        OLIVIEW_GL_ASSERT_NO_ERROR();
    }
    
    bool Window::ShouldClose() const {
        return true;
    }
    
    void Window::FocusNext() {
        Ptr<View> focus = focused_view();
        while (true) {
            focus = GetNextFocusView(focus);
            if (!focus) {
                break;
            }
            if (focus->focusable() && focus->visible()) {
                break;
            }
        }

        _Focus(focus);
    }
    
    void Window::FocusPrev() {
        Ptr<View> focus = focused_view();
        while (true) {
            focus = GetPrevFocusView(focus);
            if (!focus) {
                break;
            }
            if (focus->focusable() && focus->visible()) {
                break;
            }
        }

        _Focus(focus);
    }
    
    void Window::UnfocusView() {
        _Focus(nullptr);
    }
    
    void Window::LayoutContentView(NVGcontext *,
                                   const Ptr<WindowContentView> &)
    {}
    
    void Window::HandleMouseEvent(const MouseEvent & event_) {
        if (mouse_target_) {
            switch (event_.type()) {
                case MouseEventType::Up: {
                    RHETORIC_ASSERT(event_.button().presented());

                    MouseEvent event = mouse_target_->_ConvertMouseEventFromWindow(event_);

                    if (mouse_source_button_ == event.button()) {
                        mouse_target_->OnMouseUpEvent(event);
                        
                        mouse_target_ = nullptr;
                        mouse_source_button_ = None();
                    }
                    break;
                }
                case MouseEventType::Move: {
                    MouseEvent event = mouse_target_->_ConvertMouseEventFromWindow(event_);

                    mouse_target_->OnMouseMoveEvent(event);
                    break;
                }
                case MouseEventType::Down:
                case MouseEventType::Cancel:
                    break;
            }
        } else {
            switch (event_.type()) {
                case MouseEventType::Down: {
                    RHETORIC_ASSERT(event_.button().presented());
                    MouseEvent event = root_view_->_ConvertMouseEventFromWindow(event_);
                    Ptr<View> hit_target = root_view_->MouseHitTest(event.pos());
                    if (hit_target) {
                        PostMouseDownEventTo(event, hit_target);
                    }
                    break;
                }
                case MouseEventType::Move: {
                    MouseEvent event = root_view_->_ConvertMouseEventFromWindow(event_);
                    BroadcastMouseMoveEventTo(event, root_view_);
                    break;
                }
                case MouseEventType::Up:
                case MouseEventType::Cancel:
                    break;
            }
        }
    }
    
    void Window::HandleScrollEvent(const ScrollEvent & event) {
        Ptr<View> hit_target = root_view_->MouseHitTest(event.pos());
        if (hit_target) {
            PostScrollEventTo(event, hit_target);
        }
    }
    
    void Window::HandleKeyEvent(const KeyEvent & event) {
        Ptr<View> target = focused_view();
        if (!target) {
            target = root_view_;
        }
        bool consumed = false;
        while (target) {
            switch (event.type()) {
                case KeyEventType::Down:
                    consumed = target->OnKeyDownEvent(event);
                    break;
                case KeyEventType::Up:
                    consumed = target->OnKeyUpEvent(event);
                    break;
                case KeyEventType::Repeat:
                    consumed = target->OnKeyRepeatEvent(event);
                    break;
            }
            if (consumed) {
                return;
            }
            target = target->parent();
        }
        switch (event.type()) {
            case KeyEventType::Down:
                OnKeyDownEvent(event);
                break;
            case KeyEventType::Up:
                OnKeyUpEvent(event);
                break;
            case KeyEventType::Repeat:
                OnKeyRepeatEvent(event);
                break;
        }
    }
    
    void Window::OnKeyDownEvent(const KeyEvent &) {
    }
    void Window::OnKeyUpEvent(const KeyEvent &) {
    }
    void Window::OnKeyRepeatEvent(const KeyEvent &) {
    }
    
    void Window::Update() {
        MakeContextCurrent();
        NVGcontext * ctx = BeginNVG();
        Layout(ctx);
        Draw(ctx);
        EndNVG(ctx);
        Swap();
    }
    
    NVGcontext * Window::BeginNVG() {
        RHETORIC_ASSERT(nvg_context_ == nullptr);
        
        auto app = application();
        auto ctx = app->_nvg_context();

        nvgBeginFrame(ctx,
                      (int)window_size_.width(),
                      (int)window_size_.height(),
                      pixel_ratio_);
        
        nvg_context_ = ctx;
        
        return ctx;
    }

    void Window::Layout(NVGcontext * ctx) {
        _Layout(ctx);
    }
    
    void Window::Draw(NVGcontext * ctx) {
        _Draw(ctx);
    }
    
    void Window::EndNVG(NVGcontext * ctx) {
        RHETORIC_ASSERT(nvg_context_ == ctx);
        
        nvgEndFrame(ctx);
        nvg_context_ = nullptr;
    }
    
    void Window::Swap() {
        glfwSwapBuffers(glfw_window_);
    }
    
    void Window::_InvokeUpdate() {
        _MayClose();
        if (closed()) { return; }
        Update();
    }
    
    void Window::_UpdateAnimation(float delta_time) {
        if (closed()) { return; }
        
        root_view_->_UpdateAnimation(delta_time);
    }
    
    void Window::_MayClose() {
        if (closed()) { return; }
        
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
            CancelMouseEvent();
        }
        
        if (view->focused()) {
            view->Unfocus();            
        }
    }
    
    void Window::_Focus(const Ptr<View> & view) {
        auto old = focused_view();
        if (old) {
            focused_view_ = nullptr;
            old->_SetFocused(false);
            old->OnUnfocus();
        }
        
        if (view) {
            focused_view_ = view;
            view->_SetFocused(true);
            view->OnFocus();
        }
    }
    
    void Window::CancelNVG(NVGcontext * ctx) {
        RHETORIC_ASSERT(nvg_context_ == ctx);

        nvgCancelFrame(ctx);
        nvg_context_ = nullptr;
    }
    
    void Window::_Layout(NVGcontext * ctx) {
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
    
    void Window::_Draw(NVGcontext * ctx) {
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
        
        MayUpdatePixelRatio();
        
        if (root_view_) {
            root_view_->set_frame(Rect(Vector2(0, 0), window_size_));
        }
    }
    
    void Window::set_framebuffer_size(const Size & value) {
        framebuffer_size_ = value;
        
        MayUpdatePixelRatio();
    }
    
    void Window::MayUpdatePixelRatio() {
        if (framebuffer_size_.width() != 0 && window_size_.width() != 0) {
            pixel_ratio_ = framebuffer_size_.width() / window_size_.width();
        }
    }

    Ptr<View> Window::GetNextFocusView(const Ptr<View> & view_) const {
        Ptr<View> view = view_;
        if (!view) {
            return root_view_;
        }
        
        auto child = view->GetFirstFocusChild();
        if (child) {
            return child;
        }
        
        while (true) {
            auto parent = view->parent();
            if (!parent) {
                return nullptr;
            }
            auto sib = parent->GetNextFocusChild(view);
            if (sib) {
                return sib;
            }
            view = parent;
        }
    }
    
    Ptr<View> Window::GetPrevFocusView(const Ptr<View> & view_) const {
        Ptr<View> view = view_;
        
        if (!view) {
            view = root_view_;
        } else {
            auto parent = view->parent();
            if (!parent) {
                return nullptr;
            }
            view = parent->GetPrevFocusChild(view);
            if (!view) {
                return parent;
            }
        }
        
        while (true) {
            auto child = view->GetLastFocusChild();
            if (!child) {
                return view;
            }
            view = child;
        }
    }
    
    void Window::PostMouseDownEventTo(const MouseEvent & event, const Ptr<View> & view_) {
        Ptr<View> view = view_;
        
        bool consumed;
        while (true) {
            if (!view) {
                break;
            }
            
            MouseEvent view_event = view->_ConvertMouseEventFromWindow(event);
            consumed = view->OnMouseDownEvent(view_event);
            if (consumed) {
                mouse_target_ = view;
                mouse_source_button_ = event.button();
                return;
            }
            view = view->parent();
        }
    }
    
    
    void Window::BroadcastMouseMoveEventTo(const MouseEvent & window_event, const Ptr<View> & view) {
        MouseEvent event = view->_ConvertMouseEventFromWindow(window_event);
        view->OnMouseMoveEvent(event);
        
        for (auto & child : view->children()) {
            BroadcastMouseMoveEventTo(window_event, child);
        }
    }
    
    void Window::CancelMouseEvent() {
        mouse_target_->OnMouseCancelEvent();
        mouse_target_ = nullptr;
        mouse_source_button_ = None();
    }
    
    void Window::PostScrollEventTo(const ScrollEvent & event, const Ptr<View> & view_) {
        Ptr<View> view = view_;
        
        bool consumed;
        while (true) {
            if (!view) {
                break;
            }
            
            ScrollEvent view_event = view->_ConvertScrollEventFromWindow(event);
            consumed = view->OnScrollEvent(view_event);
            if (consumed) {
                return;
            }
            view = view->parent();
        }
    }

    void Window::RefreshHandler(GLFWwindow * window) {
        auto thiz = (Window *)glfwGetWindowUserPointer(window);
        thiz->_InvokeUpdate();
    }

    void Window::WindowSizeHandler(GLFWwindow * window, int w, int h) {
        auto thiz = (Window *)glfwGetWindowUserPointer(window);
        thiz->set_window_size(Size(w, h));
    }

    void Window::FramebufferSizeHandler(GLFWwindow * window, int w, int h) {
        auto thiz = (Window *)glfwGetWindowUserPointer(window);
        thiz->set_framebuffer_size(Size(w, h));
    }
    
    void Window::FocusHandler(GLFWwindow * window, int focused) {
        auto thiz = (Window *)glfwGetWindowUserPointer(window);
        if (focused) {
            thiz->application()->_OnWindowMoveToFront(thiz->shared_from_this());
        }
    }
    
    Vector2 Window::mouse_position() const {
        double x, y;
        glfwGetCursorPos(glfw_window_, &x, &y);
        return Vector2((float)x, (float)y);
    }
    
    void Window::MouseButtonHandler(GLFWwindow * window, int button, int action, int modifier) {
        auto thiz = (Window *)glfwGetWindowUserPointer(window);
        MouseEvent event;
        if (action == GLFW_PRESS) {
            event.set_type(MouseEventType::Down);
        } else if (action == GLFW_RELEASE) {
            event.set_type(MouseEventType::Up);
        } else {
            return;
        }
        event.set_pos(thiz->mouse_position());
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
    
    void Window::ScrollHandler(GLFWwindow * window, double x, double y) {
        auto thiz = (Window *)glfwGetWindowUserPointer(window);
        x *= -10.0;
        y *= -10.0;
        ScrollEvent event;
        event.set_scroll(Vector2((float)x, (float)y));
        event.set_pos(thiz->mouse_position());
        thiz->HandleScrollEvent(event);
    }
    
    void Window::KeyHandler(GLFWwindow * window, int key, int scancode, int action, int modifier) {
        auto thiz = (Window *)glfwGetWindowUserPointer(window);
        
        KeyEvent event;
        if (action == GLFW_PRESS) {
            event.set_type(KeyEventType::Down);
        } else if (action == GLFW_RELEASE) {
            event.set_type(KeyEventType::Up);
        } else if (action == GLFW_REPEAT) {
            event.set_type(KeyEventType::Repeat);
        }
        event.set_key(key);
        event.set_scancode(scancode);
        event.set_modifier(modifier);
        
        thiz->HandleKeyEvent(event);
    }
    
    void Window::CharHandler(GLFWwindow * window, unsigned int code) {
        auto thiz = (Window *)glfwGetWindowUserPointer(window);

        auto target = thiz->focused_view();
        if (!target) {
            return;
        }
        CharEvent event;
        event.set_unicode(static_cast<uint32_t>(code));
        
        target->OnCharEvent(event);
    }
}
