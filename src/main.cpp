#include <iostream>
#include <fstream>
#include "thread_pool/fixed_thread_pool.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "hiredis/hiredis.h"
#include "thread_pool/safe_queue.h"
#include "thread_pool/semaphore.h"
#include "librdkafka/rdkafkacpp.h"
#include "win32/wingetopt.h"
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


class ExampleEventCb : public RdKafka::EventCb {
public:
    void event_cb (RdKafka::Event &event) {
        switch (event.type())
        {
            case RdKafka::Event::EVENT_ERROR:
                if (event.fatal()) {
                    std::cerr << "FATAL ";
                    run = 0;
                }
                std::cerr << "ERROR (" << RdKafka::err2str(event.err()) << "): " <<
                          event.str() << std::endl;
                break;

            case RdKafka::Event::EVENT_STATS:
                std::cerr << "\"STATS\": " << event.str() << std::endl;
                break;

            case RdKafka::Event::EVENT_LOG:
                spdlog::error("{}", event.str().c_str());
                break;

            default:
                std::cerr << "EVENT " << event.type() <<
                          " (" << RdKafka::err2str(event.err()) << "): " <<
                          event.str() << std::endl;
                break;
        }
    }
};
void commandGuid(int argc, char **argv){
    fprintf(stdout,
            "Usage: %s [-c|--config <path>]\n"
            "       %s [-v|--version]\n"
            "       %s [-h|--help]\n"
            "\n"
            "Options:\n"
            "  -c, --config FILE      Config file path\n"
            "                         (default: lightdelay.yml)\n"
            "  -v, --version          Print package version\n"
            "  -h, --help             Show command guid\n"
            , argv[0],argv[0],argv[0]);
}
int main(int argc, char **argv) {
    int opt;
    char *configPath = nullptr;
    while ((opt = getopt(argc, argv, "cvh")) != -1) {
        switch (opt) {
            case 'c':
                std::cout << "abc: " << opt << std::endl;
                configPath = "abc";
                break;
            case 'h':
            default:
                commandGuid(argc,argv);
                return 0;
        }
    }
    if (configPath == nullptr){
        configPath = "./lightdelay.yml";
    }
    retry:
    std::ifstream fin(configPath);
    if (!fin && strcmp(configPath,"./config/lightdelay.yml") != 0){
        configPath = "./config/lightdelay.yml";
        goto retry;
    }else if(strcmp(configPath,"./config/lightdelay.yml") == 0) {
        configPath = nullptr;
    }
    if (configPath == nullptr){
        commandGuid(argc,argv);
        return 0;
    }

//    commandGuid(argc,argv);

//    std::string brokers = "localhost:9093";
//    const char *topic = "test";
//    std::string errstr;
//    RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
//    RdKafka::Conf *tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);
//    if (conf->set("bootstrap.servers", brokers, errstr) != RdKafka::Conf::CONF_OK) {
//        spdlog::error("Set kafka config bootstrap.servers error: {}", errstr);
//        return 1;
//    }
//    ExampleEventCb ex_event_cb;
//    conf->set("event_cb", &ex_event_cb, errstr);
//
//    signal(SIGINT, sigterm);
//    signal(SIGTERM, sigterm);
//
//    ExampleDeliveryReportCb ex_dr_cb;
//
//    if (conf->set("dr_cb", &ex_dr_cb, errstr) != RdKafka::Conf::CONF_OK) {
//        std::cerr << errstr << std::endl;
//        exit(1);
//    }
//
//
//    RdKafka::Producer *producer = RdKafka::Producer::create(conf, errstr);
//    if (!producer) {
//        spdlog::error("Failed to create kafka producer: {}", errstr);
//        return 1;
//    }
//    delete conf;
//    for (std::string line; run && std::getline(std::cin, line);) {
//        if (line.empty()) {
//            spdlog::error("line is empty");
//            producer->poll(0);
//            continue;
//        }
//        RdKafka::ErrorCode err = producer->produce(
//                topic,  RdKafka::Topic::PARTITION_UA,
//                RdKafka::Producer::RK_MSG_COPY, const_cast<char *>(line.c_str()),
//                line.size(), NULL, 0,0,NULL,NULL);
//        if (err != RdKafka::ERR_NO_ERROR){
//            spdlog::error("Failed to produce to topic: {}, error: {}",topic, RdKafka::err2str(err));
//        }
//        producer->poll(0);
//    }
//    producer->flush(10*1000 /* wait for max 10 seconds */);
//    delete producer;
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
