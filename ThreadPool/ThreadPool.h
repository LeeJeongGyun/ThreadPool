#pragma once

class ThreadPool
{
public:
    ThreadPool(size_t threadNum);
    ~ThreadPool();

    template<typename C, typename... Args>
    std::future<std::invoke_result_t<C, Args...>> EnqueueJob(C&& callable, Args&&... args);
    void Exit();

private:
    void WorkerThread();

private:
    size_t _numOfThread;
    std::vector<std::thread> _threads;
    bool _allThreadExit;

private:
    std::queue<std::function<void(void)>> _jobs;
    std::mutex _jobMutex;
    std::condition_variable _jobCV;

};

template <typename C, typename... Args>
inline std::future<std::invoke_result_t<C, Args...>> ThreadPool::EnqueueJob(C&& callable, Args&&... args)
{
    if (_allThreadExit)
    {
        throw std::runtime_error("Thread Pool Exit");
    }

    using result_type = std::invoke_result_t<C, Args...>;

    auto sJobPtr = std::make_shared<std::packaged_task<result_type()>>(std::bind(std::forward<C>(callable), std::forward<Args>(args)...));
    std::future<result_type> ret = sJobPtr->get_future();

    {
        std::lock_guard<std::mutex> lockGuard(_jobMutex);
        _jobs.push([sJobPtr]() { (*sJobPtr)(); });
    }

    _jobCV.notify_one();

    return ret;
}
