#include "pch.h"
#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t threadNum) : _numOfThread(threadNum), _allThreadExit(false)
{
    _threads.reserve(threadNum);
    for (int i = 0; i < threadNum; ++i)
    {
        _threads.push_back(std::thread([this]() { this->WorkerThread(); }));
    }
}

void ThreadPool::WorkerThread()
{
    while (true)
    {
        std::unique_lock<std::mutex> ulock(_jobMutex);
        _jobCV.wait(ulock, [this]() { return !_jobs.empty() || _allThreadExit == true; });
        if (_jobs.empty() && _allThreadExit)
        {
            ulock.unlock();
            break;
        }
        
        // 하나의 스레드만 실행
        std::function<void(void)> job = _jobs.front();
        _jobs.pop();
        ulock.unlock();

        job();
    }
}

void ThreadPool::EnqueueJob(std::function<void(void)> job)
{
    if (_allThreadExit)
        return;

    {
        std::lock_guard<std::mutex> lockGuard(_jobMutex);
        _jobs.push(job);
    }

    _jobCV.notify_one();
}

void ThreadPool::Exit()
{
    if (_allThreadExit)
        return;

    _allThreadExit = true;
    _jobCV.notify_all();
}

ThreadPool::~ThreadPool()
{
    Exit();
    for (std::thread &t : _threads)
        t.join();

    _threads.clear();
}
