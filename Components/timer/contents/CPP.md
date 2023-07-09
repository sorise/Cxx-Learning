### [C++ Timer 定时器实现例子](#)

**介绍**： 基于红黑树的实现

-----



```cpp
//
// Created by remix on 23-6-23.
// linux 不是有定时器信号
#include <iostream>
#include <chrono>
#include <limits>
#include <functional>
#include <map>
#include <set>
#include <utility>


class TimeNodeBase{
private:
    uint64_t ID; //标识ID
    time_t expire;  //啥时候过期
public:
    TimeNodeBase(uint64_t Id, time_t exp):ID(Id),expire(exp){

    }
    uint64_t getID() const{
        return ID;
    }
    time_t getExpire() const{
        return expire;
    }
};

static bool operator < (const TimeNodeBase &me, const TimeNodeBase &other){
    if (me.getExpire() < other.getExpire()){
        return true;
    }else if(me.getExpire() > other.getExpire()){
        return false;
    }
    return me.getID() < other.getID(); //前插入的在前面 后插入的放在后面
}


class TimeNode: public TimeNodeBase{
public:
    using CallBack = std::function<void()>;
    CallBack callBack; //毁掉函数

    TimeNode(uint64_t Id,CallBack cb, time_t exp):TimeNodeBase(Id, exp),callBack(std::move(cb)){}
};


class Timer{
    static uint64_t initID;
    std::set<TimeNode, std::less<>> nodes;
public:
    /* 获得当前时间 */
    static time_t GetTick(){
        std::chrono::time_point<std::chrono::system_clock> tp =
                std::chrono::time_point_cast<std::chrono::milliseconds>(
                        std::chrono::system_clock::now()
                );
        return std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count();
    }

    static uint64_t GenTimeTaskID(){
        return  ++initID;
    }

    template<class F, class ...Args >
    TimeNodeBase setTimeout(long long milliseconds, F && f, Args&... args){
        TimeNode::CallBack callBack = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        TimeNode tNode(GenTimeTaskID() ,callBack, GetTick() + milliseconds);
        nodes.insert(tNode);
        return static_cast<TimeNodeBase>(tNode);
    }

    bool clearTimeout(TimeNodeBase &nodeBase){
        auto it = nodes.find(nodeBase);
        if (it != nodes.end()){
            nodes.erase(it);
            return true;
        }
        return false;
    }

    time_t checkTimeout(){
        if (nodes.empty()) return -1;
        time_t diff = nodes.begin()->getExpire() - Timer::GetTick();
        return diff > 0? diff:0;
    }

    bool runTask(){
        if (!nodes.empty()){
            auto it = nodes.begin();
            time_t diff = it->getExpire() - Timer::GetTick();
            if (diff <= 0){
                it->callBack();
                nodes.erase(it);
                return true;
            }
        }
        return false;
    }
};

uint64_t Timer::initID = 0;


int main() {

    return 0;
}
```

