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

    void Application::set_finish(const std::function<void()> & value) {
        finish_ = value;
    }

    void Application::Run() {
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

        Finish();
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

    void Application::Main(const std::function<void()> & init,
                           const std::function<void()> & finish) {
        auto app = shared();
        app->set_init(init);
        app->set_finish(finish);
        app->Run();
    }

    void Application::Init() {
        if (!glfwInit()) {
            Fatal("glfwInit failed");
        }

        glfwSetTime(0.0);

        if (init_) {
            init_();
        }
    }

    void Application::Finish() {
        if (finish_) {
            finish_();
        }

        glfwTerminate();
    }

    std::mutex Application::static_mutex_;
    Ref<Application> Application::shared_;
}
