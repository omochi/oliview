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

        void set_init(const std::function<void()> & value);
        void set_final(const std::function<void()> & value);

        void Run();

        void AddWindowInternal(const Ref<Window> & window);
        void RemoveWindowInternal(const Ref<Window> & window);

        static Ref<Application> shared();
    private:
        void Init();
        void Final();

        Ref<TaskQueue> task_queue_;
        std::vector<Ref<Window>> windows_;

        std::function<void()> init_;
        std::function<void()> final_;

        static std::mutex static_mutex_;
        static Ref<Application> shared_;
    };
}
