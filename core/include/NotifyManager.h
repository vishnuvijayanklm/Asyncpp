#ifndef NOTIFYMANAGER_H
#define NOTIFYMANAGER_H

#include <core/include/Subsystem.h>
#include <util/include/defines.h>
#include <core/include/Notifier.h>
#include <core/include/Synchronizer.h>

#define SYNC_DISABLED (unsigned int)-1
#ifndef NOTIFIER_COUNT
#define NOTIFIER_COUNT 5
#endif

namespace Core
{
	class NotifyManager
	{
		private:
			volatile unsigned int m_SyncKey;
		
			Notifier *pNotifier;
		
			/*	
#ifdef WITH_EVENTPOOL	
StlQueue<IEventInfo*> mEventPoolQ;
#endif*/

			NotifyManager();

		public:

			static NotifyManager* getInstance();
			virtual ~NotifyManager();
			virtual void init();

			inline bool dispatch(shared_ptr<Async::ITaskInfo> &task); 
	};

	bool NotifyManager::dispatch(shared_ptr<Async::ITaskInfo> &task)
	{
		return pNotifier[this->m_SyncKey++ % NOTIFIER_COUNT ].addTask(task);
	}
}

#endif //NOTIFYMANAGER_H
