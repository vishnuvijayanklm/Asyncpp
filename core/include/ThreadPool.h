#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <Subsystem.h>
#include <thread>
#include <Stl.h>
#include <Exception.h>
#include <Thread.h>
using namespace std;
#define MIN_THREAD_POOL_SIZE 0x02
#define MAX_THREAD_POOL_SIZE 0xff
namespace Core
{

	class ThreadPool:public ISubsystem
	{	
		private:
			size_t m_minSize;
			size_t m_maxSize;	
			StlQueue<Thread*> m_FreePool;
			StlQueue<Thread*> m_UsedPool;
		public:
        		ThreadPool(int minSize = 2,int maxSize = 5);
        		virtual ~ThreadPool();
        		virtual void init();
        		virtual void shutdown();
			void initializeThreads() throw(INVALID_THREAD_SIZE_CONFIGURED);
			Thread* getThreadFromPool() throw(THREAD_POOL_OUTAGE,INVALID_THREAD_PTR);
	};
}
#endif
