#include "./application.h"

namespace oliview {
    Application::Application() {
        OLIVIEW_ASSERT(shared_ == nullptr);

        task_queue_.Attach(new TaskQueue());
    }

    Application::~Application()
    {
    }

    Ref<TaskQueue> Application::task_queue() const {
        return task_queue_;
    }

    void Application::set_init(const std::function<void()> & value) {
        init_ = value;
    }

    void Application::set_final(const std::function<void()> & value) {
        final_ = value;
    }

    void Application::Run() {
        glfwSwapInterval(0);
        
        Init();

        while (true) {
            if (windows_.size() == 0) {
                break;
            }

            glfwPollEvents();

            task_queue_->Run();

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

        Final();
    }

    void Application::AddWindowInternal(const Ref<Window> & window) {
        windows_.push_back(window);
    }
    void Application::RemoveWindowInternal(const Ref<Window> & window) {
        windows_ = ArrayRemove(windows_,
                               [&](const Ref<Window> & x) {
                                   return x == window;
                               });
    }

    Ref<Application> Application::shared() {
        std::lock_guard<std::mutex> lock(static_mutex_);
        if (!shared_) {
            shared_.Attach(new Application());
        }
        return shared_;
    }

    void Application::Init() {
        if (!glfwInit()) {
            Fatal("glfwInit failed");
        }

        if (init_) {
            init_();
        }
    }

    void Application::Final() {
        if (final_) {
            final_();
        }

        glfwTerminate();
    }

    std::mutex Application::static_mutex_;
    Ref<Application> Application::shared_;
}
