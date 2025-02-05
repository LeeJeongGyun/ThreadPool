
// Compiler Option C++20

#include "pch.h"
#include "ThreadPool.h"

void Work(int sleepTime, int threadId)
{
    std::cout << "Start ThreadId: " << threadId << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(sleepTime));
    std::cout << "End ThreadId: " << threadId << std::endl;
}

int main()
{
    std::cout << "Main Thread Start" << std::endl;

#ifdef _DEBUG
    int Flags = _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF;
    _CrtSetDbgFlag(Flags);
#endif

    ThreadPool threadPool(4);

    for (int i = 0; i < 10; i++)
    {
        threadPool.EnqueueJob([i]() { Work(i % 3 + 1, i); });
    }

#ifdef _DEBUG
    _ASSERT(_CrtCheckMemory());
#endif

    std::cout << "Main Thread End" << std::endl;
    return 0;
}
