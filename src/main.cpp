#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include "thread_pool/fixed_thread_pool.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "hiredis/hiredis.h"
#include "thread_pool/safe_queue.h"
#include "thread_pool/semaphore.h"
#include "librdkafka/rdkafkacpp.h"
#include "win32/wingetopt.h"
#include "ini/cpp/INIReader.h"
#include "util/timeutil.h"
#include "redis_consumer.h"
void r1(){
    spdlog::error("r1 start");
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    spdlog::error("r1 end");
}
void r2(char *abc){
    char *def = "def";
    strcpy(abc,def);
//    std::strcpy(abc,def);
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
            "Usage: %s [-c <path>]\n"
            "       %s -v | -h\n"
            "\n"
            "Options:\n"
            "  -c FILE      Config file path\n"
            "               (default: lightdelay.ini)\n"
            "  -v           Print package version\n"
            "  -h           Show command guid\n"
            , argv[0],argv[0],argv[0]);
}

int loadConfigFile(const char *configFilePath){
    if (configFilePath != nullptr){
        std::ifstream fin(configFilePath);

        return !!fin;
    }
    return false;
}
int main(int argc, char **argv) {
    int opt;
    std::string configFilePath;
    while ((opt = getopt(argc, argv, "c:vh")) != -1) {
        switch (opt) {
            case 'c':
                std::cout << "abc: " << opt << std::endl;
                configFilePath = "abc";
                break;
            case 'h':
            default:
                commandGuid(argc,argv);
                return 0;
        }
    }
    configFilePath = "./lightdelay.ini";
    INIReader reader(configFilePath);
    if (reader.ParseError() != 0){
        spdlog::error("Can't load '{}'", configFilePath);
    }
    std::string redisHost = reader.Get("redis","host","127.0.0.1");
    int redisPort = reader.GetInteger("redis","port",6379);
    std::string redisListenKeys = reader.Get("redis","listen_keys","lightdelay_default");
    std::string kafkaHost = reader.Get("kafka","host","127.0.0.1");
    std::string kafkaPort = reader.Get("kafka","port","9093");
//    char script[255];
//    FixedThreadPool *fixedThreadPool = new FixedThreadPool(64);
//    void (*rr1)() = r1;
//    fixedThreadPool->Execute(&rr1);
//    fixedThreadPool->Execute(&rr1);
//    fixedThreadPool->Execute(&rr1);
//    fixedThreadPool->Execute(&rr1);
//    fixedThreadPool->Execute(&rr1);
//    fixedThreadPool->Execute(&rr1);
//    fixedThreadPool->Execute(&rr1);
//    fixedThreadPool->Execute(&rr1);
//    fixedThreadPool->Execute(&rr1);
//    fixedThreadPool->Execute(&rr1);
//    fixedThreadPool->Execute(&rr1);
//    delete fixedThreadPool;
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

    RedisConsumer *consumer = new RedisConsumer(redisHost.c_str(),redisPort);
    if(consumer->connect()){
        while (true){
            char buf[255];
            if (consumer->poll(redisListenKeys.c_str(), buf,1000)){
                std::string c = buf;
                spdlog::error("r: {}", c);
            }
        }
    }
    delete consumer;
    return 0;
}
