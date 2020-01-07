#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <vector>
#include <queue>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

#define  MAX_THREAD_NUM 256

//线程池,可以提交变参函数或拉姆达表达式的匿名函数执行,可以获取执行返回值
//不支持类成员函数, 支持类静态成员函数或全局函数,Opteron()函数等
class CThreadPool
{
	using Task = std::function<void()>;
	// 线程池
	std::vector<std::thread> m_vecPool;
	// 任务队列
	std::queue<Task> m_queTasks;
	// 同步
	std::mutex m_lock;
	// 条件阻塞
	std::condition_variable cv_task;
	// 是否关闭提交
	std::atomic<bool> m_bStoped;
	//空闲线程数量
	std::atomic<int>  m_nIdlThrNum;
public:
	CThreadPool();
	~CThreadPool();
	void InitThreadPool(uint8_t nThreadPool);
	void DestroyThreadPool();
	template<class F, class... Args>
	auto commit(F&& f, Args&&... args) ->std::future<decltype(f(args...))>;
	inline int idlCount();
};

#include "ThreadPool.inl"
#endif