#include <core/include/Thread.h>
#include <unistd.h>
#include <util/include/defines.h>
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
		/*
		while(this->isAlive())
		{
			try
			{
				unique_ptr<IEventInfo> pEvent;
				while(this->m_Queue.pop(pEvent,true))
				{
					if(likely(pEvent.get() != nullptr))
					{
						pEvent->processEvent();
					}
					else
					{
						break;
					}
				}
			}
			catch(Exception &e)
			{

			}
			catch(exception &e)
			{

			}
			catch(...)
			{

			}
		}*/
	}
		
	void Thread::onStop()
	{
	}
}
