#include "fixed_thread_pool.h"
#include <thread>
#include <iostream>

FixedThreadPool::Worker::Worker(Runner* runner, FixedThreadPool* threadPool)
    : m_runner(runner), m_threadPool(threadPool){
}

void FixedThreadPool::Worker::Run() {
    m_threadPool->RunWorker(this);
}
FixedThreadPool::FixedThreadPool(unsigned int pool_size)
    : m_pool_size(pool_size) {
    // 初始化上下文
    m_ctl = CtlOf(RUNNING,0);
    m_worker_queue = new SafeQueue();
    m_semaphore = new Semaphore();
}
void FixedThreadPool::Execute(Runner* runner) {
    int wc = WorkerCountOf(m_ctl);
    if (wc < m_pool_size){
        if (AddWorker(runner, true))
            return;
    }
    if (IsRunning(m_ctl)){
        m_worker_queue->Push(runner);
        if (!IsRunning(m_ctl)){
            //TODO 当状态为非运行时，需要移除队列，并拒绝加入
        }else if (WorkerCountOf(m_ctl) == 0){
            AddWorker(nullptr, false);
        }
    }
}
int FixedThreadPool::AddWorker(Runner* runner, int is_core) {
    for (;;){
        // 校验状态，并且队列不能为空
        if ((m_ctl >= SHUTDOWN)
            && ((m_ctl >= STOP)
                || runner != nullptr
                || m_worker_queue->Empty())){
            return false;
        }
        for (;;) {
            // 判断当前线程数量是否大于额定值
            if (WorkerCountOf(m_ctl) >= (m_pool_size & COUNT_MASK)){
                return false;
            }
            m_ctl += 1;
            int wc = WorkerCountOf(m_ctl);
            goto next;
        }
    }
    next:
    Worker* worker = new Worker(runner,this);
    if (IsRunning(m_ctl) || ((m_ctl < STOP) && runner == nullptr)){
        m_workers.push_back(worker);
        worker->Run();
        std::thread r(&Worker::Run, worker);
//        if (r.joinable()){
//            r.join();
//        }
    }
    return 1;
}

void FixedThreadPool::RunWorker(Worker* worker){
    void (*r)() = *worker->m_runner;
    worker->m_runner = nullptr;
    m_semaphore->Notify();
    while (r != nullptr || GetTask(&r)){
        m_semaphore->Wait();
        r();
        r = nullptr;
        m_semaphore->Notify();
    }
    m_ctl += -1;
}

int FixedThreadPool::GetTask(Runner* runner){
    for (;;){
        if ((m_ctl >= SHUTDOWN)
            && ((m_ctl >= STOP)
                ||m_worker_queue->Empty())){
            m_ctl += -1;
            *runner = nullptr;
            return false;
        }
        int wc = WorkerCountOf(m_ctl);
        int timed = wc > m_pool_size;
        if (timed && (wc > 1 || m_worker_queue->Empty())) {
            m_ctl += -1;
            continue;
        }
        void (*r)() = nullptr;
        m_worker_queue->Poll(&r);
        if (r == nullptr){
            *runner = nullptr;
            return false;
        }
        *runner = r;
        return true;
    }
}

unsigned int FixedThreadPool::RunStateOf(unsigned int c) {
    return c & ~(COUNT_MASK);
}

int FixedThreadPool::IsRunning(int c) {
    return (c < SHUTDOWN);
}

int FixedThreadPool::CtlOf(int rs,int wc){
    return (rs | wc);
}
int FixedThreadPool::WorkerCountOf(int c){
    return (c & COUNT_MASK);
}