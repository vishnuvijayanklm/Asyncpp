#include <core/include/Notifier.h>
namespace Core
{
	Notifier::Notifier()
	{
		//this->start();
		//this->initialize();
		this->start();
	}

	Notifier::~Notifier()
	{
		//dtor
	}

	/*void Notifier::initialize()
	{
		ThreadPool *pThreadPool = (ThreadPool*)(pApplication->getSubsystem(THREADPOOL));
		if(likely(pThreadPool != nullptr))
		{
			pThreadPool->getThreadFromPool()->execute(bind(&Notifier::run,this));	
		}
	}*/

	void Notifier::run()
	{
		LOG_INFO((LOGGER),("Notifier started...."));	
		try
		{
			while(1)
			{
				shared_ptr<Async::ITaskInfo> pTask;
				if(this->m_Queue.pop(pTask,true))
				{
					if(likely(pTask.get() != nullptr))
					{
						pTask->executeTask();
					}
					else
					{
						break;
					}
				}
			}
		}
		catch(exception &e)
		{
		}
		catch(...)
		{
		}
		LOG_INFO((LOGGER),("Notifier stopped...."));	
	}
}
