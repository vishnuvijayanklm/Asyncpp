#ifndef NOTIFYMANAGER_H
#define NOTIFYMANAGER_H

#include <util/include/defines.h>
#include <core/include/Notifier.h>
#include <core/include/Synchronizer.h>

#ifndef NOTIFIER_COUNT
#define NOTIFIER_COUNT 25
#endif

namespace Core
{
	class NotifyManager
	{
		private:
			volatile unsigned int m_SyncKey;
		
			Notifier *pNotifier;
		
			NotifyManager();

		public:

			static NotifyManager* getInstance();
			virtual ~NotifyManager();
			virtual void init();

			inline bool dispatch(shared_ptr<Async::ITaskInfo>,SyncKey); 
			inline void printStatus();
	};

	bool NotifyManager::dispatch(shared_ptr<Async::ITaskInfo> task,SyncKey key = Core::Synchronizer::getSyncKey())
	{
		return pNotifier[key.getKey() % NOTIFIER_COUNT ].addTask(task);
	}

	void NotifyManager::printStatus()
	{	
		for(int i = 0; i< NOTIFIER_COUNT ; i++)
		{
			LOG_INFO((LOGGER),("NOTIFIER %p Pending %d",&pNotifier[i],pNotifier[i].getPendingEventsCount()));
		}
	}
}

#endif //NOTIFYMANAGER_H
