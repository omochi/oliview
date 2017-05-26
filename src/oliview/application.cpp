#include "./application.h"

namespace oliview {
    Application::Application() {
    }

    Application::~Application()
    {
    }

    void Application::Run() {
        Init();

        while (true) {
            if (windows_.size() == 0) {
                break;
            }

            glfwPollEvents();

            auto windows = windows_;
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
    }

    void Application::OnInit() {
    }

    void Application::OnFinish() {
    }

    void Application::AddWindowInternal(const Ptr<Window> & window) {
        windows_.push_back(window);
    }
    void Application::RemoveWindowInternal(const Ptr<Window> & window) {
        ArrayRemove(windows_, [&](auto x) { return x == window; });
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
