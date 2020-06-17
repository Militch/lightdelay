//
// Created by Administrator on 2020/6/17.
//

#include "redis_consumer.h"
#include "util/timeutil.h"
#include <cstring>

RedisConsumer::RedisConsumer(const char *conn_host, int conn_port)
    : m_conn_host(conn_host), m_conn_port(conn_port) {
}

bool RedisConsumer::connect() {
    struct timeval timeout = { 1, 500000 };  // 1.5 seconds
    m_redis_ctx = redisConnectWithTimeout(m_conn_host, m_conn_port,timeout);
    return !(m_redis_ctx == nullptr || m_redis_ctx->err);
}

bool RedisConsumer::poll(const char *key, char *out, unsigned int timeout) {
    if (timeout < 0)
        return false;
    long long remaining;
    std::string scriptStr = "local status, type = next(redis.call(\"TYPE\", KEYS[1])) "
            "if status ~= nil and status == \"ok\" then "
            "if type == \"zset\" then "
            "local list = redis.call(\"ZREVRANGEBYSCORE\", KEYS[1], ARGV[1], 0, \"LIMIT\", 0, 1) "
            "if list ~= nil and #list > 0 then "
            "redis.call(\"ZREM\", KEYS[1], unpack(list)) "
            "return list "
            "end end end";
    do {
        unsigned long long start = getCurrentTimestamp();
        redisReply *reply;
        void *pRedisReply;
        std::string t = std::to_string(start);
        pRedisReply = redisCommand(m_redis_ctx,"eval %s 1 %s %s",scriptStr.c_str(),key,t.c_str());
        reply = (redisReply*) pRedisReply;
        if (reply->type == REDIS_REPLY_ARRAY){
            if (reply->elements > 0) {
                char *replyStr = reply->element[0]->str;
                std::strcpy(out,replyStr);
                freeReplyObject(reply);
                return true;
            }
        }
        freeReplyObject(reply);
        unsigned long long end = getCurrentTimestamp();
        long long elapsed = (long) (end - start);
        remaining = (timeout - elapsed);
    } while (remaining > 0);
    return false;
}

RedisConsumer::~RedisConsumer() {
    redisFree(m_redis_ctx);
}

