#include <iostream>
#include "thread_pool/fixed_thread_pool.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "hiredis/hiredis.h"
#include "thread_pool/safe_queue.h"

void abc(){
    std::cout << "Pre Doing" << std::endl;
//    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Doing Finish" << std::endl;
}

int main() {
    const char *host = "192.168.1.1";

//    const char *host = "127.0.0.1";
//    redisContext *redisContext = redisConnect(host, 6379);
//    if (redisContext == nullptr || redisContext->err) {
//        if (redisContext) {
//            spdlog::error("Connect redis error: {}", redisContext->errstr);
//            return 0;
//        } else {
//            spdlog::error("Can't allocate redis context");
//            return 0;
//        }
//    }
//    void (*run)() = abc;
    Runner r = abc;
    Runner b = nullptr;
//    void (*a)() = nullptr;
//    SafeQueue* sq = new SafeQueue();
//    sq->Push(&r);
//    sq->Poll(&b);
//    b();
    FixedThreadPool* fixedThreadPool = new FixedThreadPool(1);
    void (*run)() = abc;
    fixedThreadPool->Execute(&run);
    fixedThreadPool->Execute(&run);

    return 0;
}
