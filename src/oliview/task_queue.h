#pragma once

#include "./dependency.h"
#include "./task.h"
#include "./object.h"
#include "./ref.h"

namespace oliview {
    class TaskQueue : public Object<TaskQueue> {
    public:
        void Post(const Task & task);
        void Run();
    private:
        Task FeedTask();

        std::mutex mutex_;
        std::list<Task> queue_;
    };
}
