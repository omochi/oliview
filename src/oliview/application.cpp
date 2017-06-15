#include "./application.h"

namespace oliview {
    Application::Application() {
    }

    Application::~Application()
    {
    }

    int Application::Main(int argc, char * argv[]) {
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

        return EXIT_SUCCESS;
    }

    void Application::OnInit() {
    }

    void Application::OnFinish() {
    }

    void Application::AddWindowInternal(const Ptr<Window> & window) {
        windows_.push_back(window);
    }
    void Application::RemoveWindowInternal(const Ptr<Window> & window) {
        ArrayRemoveEq(&windows_, window);
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
