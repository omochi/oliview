#include "./task_queue.h"

namespace oliview {
    void TaskQueue::Post(const Task & task) {
        std::lock_guard<std::mutex> lock(mutex_);

        queue_.push_back(task);
    }

    void TaskQueue::Run() {
        while (true) {
            Task task = FeedTask();
            if (!task) {
                break;
            }
            task();
        }
    }

    Task TaskQueue::FeedTask() {
        std::lock_guard<std::mutex> lock(mutex_);

        if (queue_.size() == 0) {
            return nullptr;
        }

        Task ret = queue_.front();
        queue_.pop_front();
        return ret;
    }
}
