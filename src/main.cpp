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

static volatile sig_atomic_t run = 1;
static void sigterm (int sig) {
    run = 0;
}

class ExampleDeliveryReportCb : public RdKafka::DeliveryReportCb {
public:
    void dr_cb (RdKafka::Message &message) {
        /* If message.err() is non-zero the message delivery failed permanently
         * for the message. */
        if (message.err())
            std::cerr << "% Message delivery failed: " << message.errstr() << std::endl;
        else
            std::cerr << "% Message delivered to topic " << message.topic_name() <<
                      " [" << message.partition() << "] at offset " <<
                      message.offset() << std::endl;
    }
};
int main() {
    std::string brokers = "abc";
    std::string errstr;
    RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    RdKafka::Conf *tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);
    if (conf->set("bootstrap.servers", brokers, errstr) != RdKafka::Conf::CONF_OK) {
        spdlog::error("Set kafka config bootstrap.servers error: {}", errstr);
        return 1;
    }
    signal(SIGINT, sigterm);
    signal(SIGTERM, sigterm);

    ExampleDeliveryReportCb ex_dr_cb;

    if (conf->set("dr_cb", &ex_dr_cb, errstr) != RdKafka::Conf::CONF_OK) {
        std::cerr << errstr << std::endl;
        exit(1);
    }

    
    RdKafka::Producer *producer = RdKafka::Producer::create(conf, errstr);
    if (!producer) {
        spdlog::error("Failed to create kafka producer: {}", errstr);
        return 1;
    }
    delete conf;
    for (std::string line; run && std::getline(std::cin, line);) {
        if (line.empty()) {
            spdlog::error("line is empty", brokers);
            producer->poll(0);
            continue;
        }
    }
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
