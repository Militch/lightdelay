#include <iostream>
#include "thread_pool/fixed_thread_pool.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "hiredis/hiredis.h"
#include "thread_pool/safe_queue.h"
#include "thread_pool/semaphore.h"
#include "librdkafka/rdkafkacpp.h"
void r1(){
    std::cout << "r1_start" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    std::cout << "r1_end" << std::endl;
}
void r2(){
    std::cout << "r2_start" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    std::cout << "r2_end" << std::endl;
}
void r3(){
    std::cout << "r3_start" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    std::cout << "r3_end" << std::endl;
}
void r4(){
    std::cout << "r4_start" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    std::cout << "r4_end" << std::endl;
}
void r5(){
    std::cout << "r5_start" << std::endl;
    std::cout << "r5_end" << std::endl;
}
void r6(){
    std::cout << "r6_start" << std::endl;
    std::cout << "r6_end" << std::endl;
}
void r7(){
    std::cout << "r7" << std::endl;
}

void r8(){
    std::cout << "r8" << std::endl;
}
void r9(){
    std::cout << "r9" << std::endl;
}

void r10(){
    std::cout << "r10" << std::endl;
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
    return 0;
}
