//
// Created by litch on 2020/6/4.
//

#ifndef LIGHTDELAY_FIXED_THREAD_POOL_H
#define LIGHTDELAY_FIXED_THREAD_POOL_H
#include "safe_queue.h"
#include "vector"
#include "thread"
#include "semaphore.h"
typedef void(*Runner)();
class FixedThreadPool {
public:
    explicit FixedThreadPool(unsigned int pool_size);
    void Execute(Runner* runner);
private:
    class Worker {
        friend class FixedThreadPool;
    public:
        Runner* m_runner;
        FixedThreadPool* m_threadPool;
        explicit Worker(Runner* runner, FixedThreadPool* threadPool);
        void Run();
    private:
    };
    unsigned int m_pool_size;
    int m_ctl;
    static const unsigned int COUNT_BITS = 29;
    static const int COUNT_MASK = (1u << COUNT_BITS) - 1;
    // 运行时状态。能接受新任务，并且也能处理阻塞队列中的任务
    static const int RUNNING = ~COUNT_MASK;
    // 关闭状态。不再接受新任务，但可以继续处理阻塞队列中的任务
    static const int SHUTDOWN = (0u << COUNT_BITS);
    // 停止状态。不再接受新任务，不处理阻塞队列中的任务，并且中断正在执行的任务
    static const int STOP = (1u << COUNT_BITS);
    // 终止状态。所有的任务都已经终止了
    static const int TIDYING = (2u << COUNT_BITS);
    static const int TERMINATED = (3u << COUNT_BITS);
    SafeQueue* m_worker_queue;
    Semaphore* m_semaphore;
    std::vector<Worker*> m_workers;
    static unsigned int RunStateOf(unsigned int c);
    /**
     * 当前线程池是否正在运行
     * @param c 状态值
     * @return 结果
     */
    static int IsRunning(int c);
    /**
     * 根据上下文标记值计算线程数量
     * @param c 标记值
     * @return 线程数量
     */
    static int WorkerCountOf(int c);
    /**
     * 计算上下文标记值
     * @param rs 运行状态
     * @param wc 线程数量
     * @return 上下文标记值
     */
    static int CtlOf(int rs, int wc);
    /**
     * 新增工作任务
     * @param runner 执行动作
     * @param is_core 是否为核心工作线程
     * @return 结果
     */
    int AddWorker(Runner* runner, int is_core);
    void RunWorker(Worker* worker);
    int GetTask(Runner* runner);
};
#endif //LIGHTDELAY_FIXED_THREAD_POOL_H
