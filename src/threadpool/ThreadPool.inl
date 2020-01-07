#include "ThreadPool.h"

#ifndef __THREAD_POOL_INL__
#define __THREAD_POOL_INL__

// 提交一个任务
// 调用.get()获取返回值会等待任务执行完,获取返回值
// 有两种方法可以实现调用类成员，
// 一. bind:  .commit(std::bind(&Dog::sayHello, &dog));
// 二. mem_f: .commit(std::mem_fn(&Dog::sayHello), &dog)
template <class F, class... Args>
auto CThreadPool::commit(F &&f, Args &&... args) -> std::future<decltype(f(args...))>
{
    if (m_bStoped.load()) // stop == true ??
        throw std::runtime_error("commit on CThreadPool is stopped.");

    using RetType = decltype(f(args...)); // typename std::result_of<F(Args...)>::type, 函数 f 的返回值类型
    auto task = std::make_shared<std::packaged_task<RetType()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    std::future<RetType> future = task->get_future();
    {                                             // 添加任务到队列
        std::lock_guard<std::mutex> lock{m_lock}; //对当前块的语句加锁  lock_guard 是 mutex 的 stack 封装类，构造的时候 lock()，析构的时候 unlock()
        m_queTasks.emplace(
            [task]() { // push(Task{...})
                (*task)();
            });
    }
    cv_task.notify_one(); // 唤醒一个线程执行  
    return future;
}

inline int CThreadPool::idlCount()
{
    return m_nIdlThrNum;
}

#endif