#include <Thread.h>
#include <unistd.h>
namespace Core
{
	Thread::Thread()
	{
		this->start();
	}

	Thread::~Thread()
	{
	}
	
	void Thread::run()
	{
		while(1)
		{
			sleep(1);
		}
	}
		
	void Thread::onStop()
	{
	}
	template<typename T>
	void Thread::execute(T fn)
	{
		this->m_Queue.push(unique_ptr<IEventInfo>(fn));
	}
}
