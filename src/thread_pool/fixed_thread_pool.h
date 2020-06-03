//
// Created by litch on 2020/6/4.
//

#ifndef LIGHTDELAY_FIXED_THREAD_POOL_H
#define LIGHTDELAY_FIXED_THREAD_POOL_H
#include "safe_queue.h"
#include "vector"
#include "thread"
typedef void(*Runner)();
class fixed_thread_pool {
public:
    explicit fixed_thread_pool(int pool_size);
    void Execute(Runner* runner);
private:
    int m_pool_size;
    int m_running;
    safe_queue* m_safe_queue;
    std::vector<std::thread> m_core_poll;
    int AddWorker(Runner* runner, int is_core);
};
#endif //LIGHTDELAY_FIXED_THREAD_POOL_H
