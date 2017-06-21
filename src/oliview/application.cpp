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

        InitFont();

        OnInit();
    }

    void Application::Finish() {
        OnFinish();

        glfwTerminate();
    }

    void Application::InitFont() {
        std::vector<FilePath> paths;
#if RHETORIC_MACOS
        paths.push_back(FilePath::home() + FilePath("Library/Fonts"));
        paths.push_back(FilePath("/Library/Fonts"));
        paths.push_back(FilePath("/System/Library/Fonts"));
#endif
        Font::set_search_paths(paths);

        Font::Find(NVGcontext *context, <#const std::string &name#>)
        "ヒラギノ角ゴシック W3"
    }
}
