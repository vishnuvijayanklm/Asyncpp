#include <core/include/NotifyManager.h>

namespace Core
{
	NotifyManager::NotifyManager()
	{
		this->pNotifier = nullptr;
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
		this->pNotifier = new Notifier[NOTIFIER_COUNT];
		LOG_VERBOSE((LOGGER),("NotifyManager::init<<<"));
	}
}
