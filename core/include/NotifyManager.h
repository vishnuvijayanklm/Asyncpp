#ifndef NOTIFYMANAGER_H
#define NOTIFYMANAGER_H

#include <util/include/defines.h>
#include <core/include/Notifier.h>
#include <core/include/Synchronizer.h>
#include <array>

#ifndef NOTIFIER_COUNT
#define NOTIFIER_COUNT 5
#endif

namespace Core
{
	class NotifyManager
	{
		private:
			volatile unsigned int m_SyncKey;
		
			std::array<Notifier*,NOTIFIER_COUNT> mpNotifier;
		
			NotifyManager();

		public:

			static NotifyManager* getInstance();
			virtual ~NotifyManager();
			virtual void init();

			inline bool dispatch(shared_ptr<Async::ITaskInfo>,SyncKey); 
			inline void printStatus();
			inline void onExit();
	};

	bool NotifyManager::dispatch(shared_ptr<Async::ITaskInfo> task,SyncKey key = Core::Synchronizer::getSyncKey())
	{
		return mpNotifier[key.getKey() % NOTIFIER_COUNT ]->addTask(task);
	}

	void NotifyManager::printStatus()
	{	
		for(Notifier *pNotifier : this->mpNotifier)
		{
			LOG_INFO((LOGGER),("NOTIFIER %p Pending %d",&pNotifier,pNotifier->getPendingEventsCount()));
		}	
	}

	void NotifyManager::onExit()
	{
		for(Notifier *pNotifier : this->mpNotifier)
                {
			pNotifier->onExit();
		}
		for(Notifier *pNotifier : this->mpNotifier)
                {
                        pNotifier->join();
                }

	}
}

#endif //NOTIFYMANAGER_H
