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
			shared_ptr<Async::ITaskInfo> pTask;
			while(this->m_Queue.pop(pTask,true))
			{
				if(likely(pTask.get() != nullptr))
				{
					pTask.get()->executeTask();
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
		LOG_INFO((LOGGER),("Notifier stopped...."));	
	}
}
