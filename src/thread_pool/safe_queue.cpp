#include "safe_queue.h"

int SafeQueue::Empty(){
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_queue.empty();
}
int SafeQueue::Size() {
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_queue.size();
}
void SafeQueue::Push(SafeQueueTask* task) {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_queue.push(*task);
}
int SafeQueue::Poll(SafeQueueTask* task) {
    std::unique_lock<std::mutex> lock(m_mutex);
    if (m_queue.empty()){
        return 0;
    }
    *task = *m_queue.front();
    m_queue.pop();
    return 1;
}