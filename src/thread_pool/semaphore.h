//
// Created by Administrator on 2020/6/8.
//

#ifndef LIGHTDELAY_SEMAPHORE_H
#define LIGHTDELAY_SEMAPHORE_H

#include <mutex>
#include <condition_variable>

class Semaphore {
public:
    void Notify();
    void NotifyAll();
    void Wait();
private:
    std::mutex m_mutex;
    std::condition_variable m_cv;
    unsigned long m_count = 0;
};


#endif //LIGHTDELAY_SEMAPHORE_H
