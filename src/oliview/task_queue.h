#pragma once

#include "./dependency.h"
#include "./task.h"

namespace oliview {
    class TaskQueue : public std::enable_shared_from_this<TaskQueue> {
    public:
        void Post(const Task & task);
        void Run();
    private:
        Task FeedTask();

        std::mutex mutex_;
        std::list<Task> queue_;
    };
}
