//
// Created by Administrator on 2020/6/17.
//

#ifndef LIGHTDELAY_REDISCONSUMER_H
#define LIGHTDELAY_REDISCONSUMER_H
#include <string>
#include "hiredis/hiredis.h"
class RedisConsumer {
public:
    RedisConsumer(const char *conn_host, int conn_port);
    ~RedisConsumer();
    bool connect();
    bool poll(const char *key,char *out,unsigned int timeout);
private:
    const char *m_conn_host;
    int m_conn_port;
    redisContext *m_redis_ctx = nullptr;
};


#endif //LIGHTDELAY_REDISCONSUMER_H
