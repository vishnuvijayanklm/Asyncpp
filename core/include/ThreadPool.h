#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <core/include/Subsystem.h>
#include <core/include/Thread.h>
#include <containers/include/Stl.h>
#include <thread>

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
			bool initializeThreads();
			Thread* getThreadFromPool();
	};
}
#endif
