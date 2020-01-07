#include "ThreadPool.h"


CThreadPool::CThreadPool()
	:m_bStoped(false)
{
	
}

CThreadPool::~CThreadPool()
{
    DestroyThreadPool();
}

void CThreadPool::InitThreadPool(uint8_t nThreadPool)
{
	m_nIdlThrNum = nThreadPool < 1 ? 1 : nThreadPool;
	for (nThreadPool = 0; nThreadPool < m_nIdlThrNum; ++nThreadPool)
	{   //初始化线程数量
		m_vecPool.emplace_back(
			[this]
		{ // 工作线程函数
			while (!this->m_bStoped)
			{
				std::function<void()> task;
				{   
					// 获取一个待执行的 task
					// unique_lock 相比 lock_guard 的好处是：可以随时 unlock() 和 lock()
					std::unique_lock<std::mutex> lock{ this->m_lock };
					// wait 直到有 task
					this->cv_task.wait(lock,
						[this] {
						return this->m_bStoped.load() || !this->m_queTasks.empty();
					});
					if (this->m_bStoped && this->m_queTasks.empty())
						return;
					task = std::move(this->m_queTasks.front()); //取一个 task
					this->m_queTasks.pop();
				}
				m_nIdlThrNum--;
				task();
				m_nIdlThrNum++;
			}
		});
	}
}

void CThreadPool::DestroyThreadPool()
{
	m_bStoped.store(true);
	cv_task.notify_all(); // 唤醒所有线程执行
	for (std::thread& thread : m_vecPool) {
		//thread.detach(); // 让线程“自生自灭”
		if (thread.joinable())
			thread.join(); // 等待任务结束， 前提：线程一定会执行完
	}
}