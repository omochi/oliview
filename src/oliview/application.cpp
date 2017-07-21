#include "./application.h"

namespace oliview {
    Application::Application():
    nvg_context_(nullptr)
    {}

    Application::~Application()
    {}

    int Application::Main(int argc, char * argv[]) {
        RHETORIC_UNUSED(argc);
        RHETORIC_UNUSED(argv);
        
        Init();

        while (true) {
            std::chrono::duration<float> delta_time(0.0f);
            auto now = std::chrono::steady_clock::now();
            if (prev_update_time_) {
                delta_time = now - *prev_update_time_;
            }
            prev_update_time_ = Some(now);
            
            glfwPollEvents();
            
            if (windows_.size() == 0) {
                break;
            }
            
            std::vector<Ptr<Window>> update_windows = windows_;
            while (true) {
                if (update_windows.size() == 0) {
                    break;
                }
                
                Ptr<Window> w = update_windows[0];
                ArrayRemoveAt(&update_windows, 0);
                
                w->MakeContextCurrent();
                w->_UpdateAnimation(delta_time.count());
                w->_InvokeUpdate();
            }
        }

        Finish();

        return EXIT_SUCCESS;
    }
    
    void Application::OnInit() {}
    void Application::OnFinish() {}
    
    NVGcontext * Application::_nvg_context() const {
        return nvg_context_;
    }

    void Application::_AddWindow(const Ptr<Window> & window) {
        if (windows_.size() == 0) {
            InitNVGContext();
        }
        
        windows_.push_back(window);
    }
    
    void Application::_RemoveWindow(const Ptr<Window> & window) {
        auto indexo = ArrayFindIndexEq(windows_, window);
        if (indexo) {
            auto index = *indexo;
            ArrayRemoveAt(&windows_, index);
            
            if (window->focused()) {
                Ptr<Window> focus;
                if (index < windows_.size()) {
                    focus = windows_[index];
                } else if (0 < index) {
                    focus = windows_[index - 1];
                }
                if (focus) {
                    focus->Focus();
                }
            }
        }

        if (windows_.size() == 0) {
            DestroyNVGContext();
        }
    }
    
    void Application::_OnWindowMoveToFront(const Ptr<Window> & window) {
        RHETORIC_ASSERT(window->application() == shared_from_this());
        
        auto index = ArrayFindIndexEq(windows_, window).value();
        ArrayRemoveAt(&windows_, index);
        windows_.push_back(window);
    }
    
    Ptr<Window> Application::_shared_context_window() const {
        if (windows_.size() == 0) {
            return nullptr;
        }
        return windows_[0];
    }

    void Application::Init() {
        if (!glfwInit()) {
            RHETORIC_FATAL("glfwInit failed");
        }

        glfwSetTime(0.0);

        OnInit();
    }

    void Application::Finish() {
        OnFinish();

        glfwTerminate();
    }
    
    void Application::InitNVGContext() {
        auto ret = _InitNVGContext();
        if (!ret) {
            RHETORIC_FATAL(ret.error()->ToString());
        }
    }
    
    Result<None> Application::_InitNVGContext() {
        auto thiz = shared_from_this();
        
        NVGcontext * nvg_context = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
        nvg_context_ = nvg_context;
        
        font_manager_ = OLIVIEW_INIT(FontManager, thiz);
        RHETORIC_TRY_VOID(font_manager_->SetUp());
        
        return Ok(None());
    }

    void Application::DestroyNVGContext() {
        if (nvg_context_) {
            nvgDeleteGL3(nvg_context_);
            nvg_context_ = nullptr;
        }
    }
    
}
