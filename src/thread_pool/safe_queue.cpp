#include "safe_queue.h"

int safe_queue::empty(){
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_queue.empty();
}
int safe_queue::size() {
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_queue.size();
}
void safe_queue::push(SafeQueueTask* task) {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_queue.push(*task);
}
int safe_queue::get(SafeQueueTask* task) {
    std::unique_lock<std::mutex> lock(m_mutex);
    if (m_queue.empty()){
        return 0;
    }
    *task = m_queue.front();
    m_queue.pop();
    return 1;
}