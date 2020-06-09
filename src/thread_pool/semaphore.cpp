//
// Created by Administrator on 2020/6/8.
//

#include "semaphore.h"

void Semaphore::Notify() {
    std::unique_lock<std::mutex> lock(m_mutex);
    ++m_count;
    m_cv.notify_one();
}
void Semaphore::Wait() {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_cv.wait(lock,[=]{return m_count > 0; });
    --m_count;
}
