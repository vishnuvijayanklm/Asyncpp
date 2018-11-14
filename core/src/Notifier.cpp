#include <Notifier.h>
namespace Core
{
	Notifier::Notifier()
	{
		//this->start();
		this->initialize();
	}

	Notifier::~Notifier()
	{
		//dtor
	}

	void Notifier::initialize()
	{
		ThreadPool *pThreadPool = (ThreadPool*)(pApplication->getSubsystem(THREADPOOL));
		if(pThreadPool != nullptr)
		{
			pThreadPool->getThreadFromPool();	
		}
	}
	void Notifier::run()
	{
		LOG_INFO((LOGGER),("Notifier started...."));	
		while(this->isAlive())
		{
			try
			{
				unique_ptr<IEventInfo> pEvent;
				while(this->m_Queue.pop(pEvent,true))
				{
					if(pEvent.get())
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
		}
		LOG_INFO((LOGGER),("Notifier stopped...."));	
	}
}
