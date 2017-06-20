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

                window->MayTryClose();
                if (window->closed()) {
                    continue;
                }

                window->Draw();
            }
        }

        Finish();

        return EXIT_SUCCESS;
    }

    void Application::OnInit() {
    }

    void Application::OnFinish() {
    }
    
    NVGcontext * Application::nvg_context() const {
        return nvg_context_;
    }

    void Application::_AddWindow(const Ptr<Window> & window) {
        if (windows_.size() == 0) {
            NVGcontext * nvg_context = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
            _set_nvg_context(nvg_context);
        }
        
        windows_.push_back(window);
    }
    
    void Application::_RemoveWindow(const Ptr<Window> & window) {
        ArrayRemoveEq(&windows_, window);
        
        if (windows_.size() == 0) {
            if (nvg_context_) {
                nvgDeleteGL3(nvg_context_);
                nvg_context_ = nullptr;
            }
        }
    }
    
    Ptr<Window> Application::_shared_context_window() const {
        if (windows_.size() == 0) {
            return nullptr;
        }
        return windows_[0];
    }
    
    NVGcontext * Application::_nvg_context() const {
        return nvg_context_;
    }
    
    void Application::_set_nvg_context(NVGcontext * value) {
        nvg_context_ = value;
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
}
