#pragma once

#include "./dependency.h"
#include "./task_queue.h"
#include "./window.h"
#include "./array_function.h"

namespace oliview {
    class Application : public std::enable_shared_from_this<Application> {
    public:
        Application();
        virtual ~Application();

        Ptr<TaskQueue> task_queue() const;

        void Run();

        virtual void OnInit();
        virtual void OnFinish();

        void AddWindowInternal(const Ptr<Window> & window);
        void RemoveWindowInternal(const Ptr<Window> & window);
    private:
        void Init();
        void Finish();

        std::vector<Ptr<Window>> windows_;
    };

    template <typename T>
    int ApplicationMain(int argc, char * argv[]);
}

#include "./application_inline.h"
