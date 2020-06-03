#include "fixed_thread_pool.h"
#include <thread>

fixed_thread_pool::fixed_thread_pool(int pool_size):
    m_pool_size(pool_size) {
    m_safe_queue = new safe_queue();
}
void fixed_thread_pool::Execute(Runner* runner) {
    if (m_core_poll.size() < m_pool_size){
        if (AddWorker(runner, true))
            return;
    }
    AddWorker(runner, false);
}
int fixed_thread_pool::AddWorker(Runner *runner, int is_core) {
    retry:
    for(;;){
        if (runner == nullptr && !m_core_poll.empty())
            return 0;
        i_retry:
        for (;;){
            int wc = m_safe_queue->size();
            if (wc >= (is_core?m_pool_size:1024))
                return 0;

        }
    }


    return 1;
}