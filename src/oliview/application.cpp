#include "./application.h"

namespace oliview {
    Application::Application():
    nvg_context_(nullptr)
    {
    }

    Application::~Application()
    {
    }

    int Application::Main(int argc, char * argv[]) {
        Init();

        while (true) {
            auto windows = windows_;

            if (windows.size() == 0) {
                break;
            }

            glfwPollEvents();

            for (auto & window : windows) {
                window->MakeContextCurrent();

                window->_MayTryClose();
                if (window->closed()) {
                    continue;
                }

                window->_Update();
            }
        }

        Finish();

        return EXIT_SUCCESS;
    }

    void Application::_AddWindow(const Ptr<Window> & window) {
        if (windows_.size() == 0) {
            InitNVGContext();
        }
        
        windows_.push_back(window);
    }
    
    void Application::_RemoveWindow(const Ptr<Window> & window) {
        ArrayRemoveEq(&windows_, window);
        
        if (windows_.size() == 0) {
            DestroyNVGContext();
        }
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
        
        RHETORIC_TRY_ASSIGN(font_manager_, FontManager::Create(thiz));
        
        return Success(None());
    }

    void Application::DestroyNVGContext() {
        if (nvg_context_) {
            nvgDeleteGL3(nvg_context_);
            nvg_context_ = nullptr;
        }
    }
    
}
