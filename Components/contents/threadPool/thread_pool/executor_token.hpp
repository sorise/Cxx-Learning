#include "executor.hpp"
#include <future>

#ifndef MUSE_THREAD_POOL_EXECUTOR_TOKEN_HPP
#define MUSE_THREAD_POOL_EXECUTOR_TOKEN_HPP  1

namespace muse::pool{

    template<typename R>
    class ExecutorToken:public Executor{
    public:
        ExecutorToken(std::function<void()> runner, std::future<R> r)
                :Executor(std::move(runner)),resultFuture(std::move(r)) {
        };

        ExecutorToken(const ExecutorToken& other) = delete;

        ExecutorToken(ExecutorToken&& other) noexcept
        :Executor(std::move(other)), resultFuture(std::move(other.resultFuture)){};

        bool isCancel(){ return this->cancelState.load(std::memory_order_release); };
        bool isFinish(){ return this->finishState; };
        bool isDiscard(){ return this->discardState; };

        //废物方法，线程不安全！不推荐使用
        void Cancel(){
            if (!this->finishState){
                cancelState.store(true ,std::memory_order_release);
            }
        }

        std::future<R> get_future(){
            return std::move(resultFuture);
        }
    private:
        std::future<R> resultFuture;
    };

    //将任务封装成一个 token！
    template< class F, class ...Args >
    auto make_executor(F&& f, Args&&...args) -> std::shared_ptr<ExecutorToken<decltype(f(args...))>>{
        using ReturnType = decltype(f(args...));

        const std::shared_ptr<std::packaged_task<ReturnType()>>  runner =
                std::make_shared<std::packaged_task<ReturnType()>>(
                        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::function<void()> package = [runner](){
            (*runner)();
        };

        std::shared_ptr<ExecutorToken<ReturnType>> tokenPtr(
            new ExecutorToken<ReturnType>(package, runner->get_future())
        );
        return tokenPtr;
    }

}


#endif //MUSE_THREAD_POOL_EXECUTOR_TOKEN_HPP
