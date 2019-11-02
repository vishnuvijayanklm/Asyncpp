#include <core/include/Notifier.h>
namespace Core
{
	Notifier::Notifier()
	{
		this->start();
	}

	Notifier::~Notifier()
	{
		//dtor
	}

	void Notifier::run()
	{
		//LOG_INFO((LOGGER),("Notifier started...."));	
		try
		{
			while(1)
			{
				Async::ITaskInfoPtr pTask;
				if(this->mQueue.pop(pTask,true))
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
		//LOG_INFO((LOGGER),("Notifier stopped...."));	
	}
}
