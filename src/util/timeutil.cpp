//
// Created by Administrator on 2020/6/17.
//

#include "timeutil.h"
#include <chrono>
unsigned long long getCurrentTimestamp(){
    using namespace std::chrono;
    auto now = system_clock::now();
    return duration_cast<milliseconds>(now.time_since_epoch()).count();
}
