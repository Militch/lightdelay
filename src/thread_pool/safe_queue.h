#ifndef LIGHTDELAY_SAFE_QUEUE_H
#define LIGHTDELAY_SAFE_QUEUE_H
#include <mutex>
#include <queue>
typedef void(*SafeQueueTask)();
class SafeQueue {
public:
    SafeQueue()= default;;
    int Empty();
    int Size();
    void Push(SafeQueueTask* task);
    int Poll(SafeQueueTask* task);

private:
    std::queue<SafeQueueTask> m_queue;
    std::mutex m_mutex;
};
#endif //LIGHTDELAY_SAFE_QUEUE_H
