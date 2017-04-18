#pragma once

#include "./dependency.h"
#include "./assert.h"
#include "./print.h"
#include "./object.h"
#include "./ref.h"
#include "./task_queue.h"
#include "./window.h"
#include "./array_function.h"

namespace oliview {
    class Application : public Object<Application> {
    public:
        Application();
        virtual ~Application();

        Ref<TaskQueue> task_queue() const;

        void Run();

        virtual void OnInit();
        virtual void OnFinish();

        void AddWindowInternal(const Ref<Window> & window);
        void RemoveWindowInternal(const Ref<Window> & window);
    private:
        void Init();
        void Finish();

        std::vector<Ref<Window>> windows_;
    };

    template <typename T>
    int ApplicationMain(int argc, char * argv[]);
}

#include "./application_inline.h"
