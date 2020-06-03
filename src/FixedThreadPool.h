//
// Created by litch on 2020/6/4.
//

#ifndef LIGHTDELAY_FIXEDTHREADPOOL_H
#define LIGHTDELAY_FIXEDTHREADPOOL_H
#include "SafeQueue.h"
#include "vector"
#include "thread"
typedef void(*Runner)();
class FixedThreadPool {
public:
    explicit FixedThreadPool(int pool_size);
    void Execute(Runner* runner);
private:
    int m_pool_size;
    int m_running;
    SafeQueue* m_safe_queue;
    std::vector<std::thread> m_core_poll;
    int AddWorker(Runner* runner, int is_core);
};
#endif //LIGHTDELAY_FIXEDTHREADPOOL_H
