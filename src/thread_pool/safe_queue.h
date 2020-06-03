#ifndef LIGHTDELAY_SAFE_QUEUE_H
#define LIGHTDELAY_SAFE_QUEUE_H
#include <mutex>
#include <queue>
typedef void(*SafeQueueTask)();
class safe_queue {
public:
    safe_queue(){};
    int empty();
    int size();
    void push(SafeQueueTask* task);
    int get(SafeQueueTask* task);

private:
    std::queue<SafeQueueTask> m_queue;
    std::mutex m_mutex;
};
#endif //LIGHTDELAY_SAFE_QUEUE_H
