#pragma once

class ThreadPool
{
public:
    ThreadPool(size_t threadNum);
    ~ThreadPool();

    void EnqueueJob(std::function<void(void)> job);
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

