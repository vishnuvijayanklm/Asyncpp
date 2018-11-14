#include "NotifyManager.h"
namespace Core
{
	NotifyManager::NotifyManager():ISubsystem(NOTIFY_MANAGER)
	{
		this->pNotifier = nullptr;
	}

	NotifyManager::~NotifyManager()
	{
		//dtor
	}

	void NotifyManager::init()
	{
		LOG_VERBOSE((LOGGER),("NotifyManager::init>>>"));
		#if NOTIFIER_COUNT < 1
			#error No notifier avaliable to process
		#endif
		this->m_SyncKey = 0;
		this->pNotifier = new Notifier[NOTIFIER_COUNT];
		LOG_VERBOSE((LOGGER),("NotifyManager::init<<<"));
	}
	void NotifyManager::shutdown()
	{
		LOG_VERBOSE((LOGGER),("NotifyManager::Shutdown>>>"));
		if(this->pNotifier)
		{
			for(int i=0;i<NOTIFIER_COUNT;i++)
			{
				//this->pNotifier[i].stop();
			}	
			delete[] this->pNotifier;
		}
		this->pNotifier = nullptr;
		LOG_VERBOSE((LOGGER),("NotifyManager::Shutdown<<<"));
	}
}
