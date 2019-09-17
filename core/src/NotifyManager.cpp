#include <core/include/NotifyManager.h>

namespace Core
{
	NotifyManager::NotifyManager()
	{
		this->init();
	}

	NotifyManager::~NotifyManager()
	{
		//dtor
	}

	NotifyManager* NotifyManager::getInstance()
	{
		static NotifyManager *pNotifyManager = new NotifyManager;
		return pNotifyManager;
	}

	void NotifyManager::init()
	{
		LOG_VERBOSE((LOGGER),("NotifyManager::init>>>"));
		#if NOTIFIER_COUNT < 1
			#error No notifier avaliable to process
		#endif
		this->m_SyncKey = 0;
		for(int i = 0 ;i< NOTIFIER_COUNT ; i++)
		{
			this->mpNotifier[i] = new Notifier;
		}
		LOG_VERBOSE((LOGGER),("NotifyManager::init<<<"));
	}
}
