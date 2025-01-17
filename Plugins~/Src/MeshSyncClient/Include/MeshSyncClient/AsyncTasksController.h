#pragma once

#include <vector>
#include <future>

namespace MeshSyncClient {

class AsyncTasksController {
public:
    template<typename T>
    void AddTask(const std::launch launchMode, T task) {
        if (launchMode == std::launch::async) {
            m_tasks.push_back(std::async(std::launch::async, task));
        }
        else {
        	// Cannot use std::async's deferred mode here, we need to ensure these deferred tasks run after everything else is complete:
            m_deferredTasks.push_back(std::function(task));
        }
    }

    void Wait();

private:

    std::vector<std::future<void>> m_tasks;
    std::vector<std::function<void()>> m_deferredTasks;
};


} // namespace MeshSyncClient
