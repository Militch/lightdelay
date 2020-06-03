#ifndef LIGHTDELAY_SAFEQUEUE_H
#define LIGHTDELAY_SAFEQUEUE_H
#include <mutex>
#include <queue>
typedef void(*SafeQueueTask)();
class SafeQueue {
public:
    SafeQueue(){};
    int empty();
    int size();
    void push(SafeQueueTask* task);
    int get(SafeQueueTask* task);

private:
    std::queue<SafeQueueTask> m_queue;
    std::mutex m_mutex;
};
#endif //LIGHTDELAY_SAFEQUEUE_H
