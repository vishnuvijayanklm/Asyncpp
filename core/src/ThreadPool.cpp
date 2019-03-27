#include <ThreadPool.h>
#include <defines.h>
namespace Core
{
	ThreadPool::ThreadPool(int minSize,int maxSize):ISubsystem(THREADPOOL)
	{
		LOG_VERBOSE((LOGGER),("ThreadPool::ThreadPool>>>"));
		this->m_minSize = minSize;
		this->m_maxSize = maxSize;
		this->initializeThreads();
		LOG_VERBOSE((LOGGER),("ThreadPool::ThreadPool<<<"));
	}
	ThreadPool::~ThreadPool()
	{
	}
	void ThreadPool::init()
	{
	}
	void ThreadPool::shutdown()
	{
	}
	void ThreadPool::initializeThreads()
	{
		LOG_VERBOSE((LOGGER),("ThreadPool::initializeThreads>>>"));
		if(this->m_minSize < MIN_THREAD_POOL_SIZE || this->m_maxSize > MAX_THREAD_POOL_SIZE)
		{
			throw INVALID_THREAD_SIZE_CONFIGURED(string("Min thread should be >= " + to_string(MIN_THREAD_POOL_SIZE) + " and max thread should be <= "+ to_string(MAX_THREAD_POOL_SIZE)));
		}

		size_t size = this->m_FreePool.size() + this->m_UsedPool.size();
		size == 0 ? size = 2:( size * 2 > this->m_maxSize) ? size = this->m_maxSize : size *= 2;

		cout<<"Size "<<size<<endl;
		cout<<"Pool size "<<this->m_FreePool.size()<<endl;

		while((this->m_FreePool.size() + this->m_UsedPool.size()) < size)
		{
			Thread *pThread = new Thread();
			this->m_FreePool.push(pThread);
		}
		LOG_VERBOSE((LOGGER),("ThreadPool::initializeThreads<<<"));
	}

	Thread* ThreadPool::getThreadFromPool()
	{
		LOG_VERBOSE((LOGGER),("ThreadPool::getThreadFromPool>>>"));
		Thread *pThread = nullptr;
		if(likely(!this->m_FreePool.pop(pThread)))
		{
			this->initializeThreads();
			if(unlikely(!this->m_FreePool.pop(pThread)))
			{
				throw THREAD_POOL_OUTAGE();
			}
		}	
		if(unlikely(pThread == nullptr))
		{
			throw INVALID_THREAD_PTR(); 	
		}
		
		this->m_UsedPool.push(pThread);
		LOG_VERBOSE((LOGGER),("ThreadPool::getThreadFromPool<<<"));
		return pThread;
	}
}
