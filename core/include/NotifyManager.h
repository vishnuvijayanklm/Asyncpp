#ifndef NOTIFYMANAGER_H
#define NOTIFYMANAGER_H

#include <Subsystem.h>
#include <defines.h>
#include <Notifier.h>
#include <future>
#define SYNC_DISABLED (unsigned int)-1
#ifndef NOTIFIER_COUNT
#define NOTIFIER_COUNT 5
#endif
namespace Core
{
	class NotifyManager : public ISubsystem
	{
		private:
			volatile unsigned int m_SyncKey;
			Notifier *pNotifier;

		protected:
		public:
			NotifyManager();
			virtual ~NotifyManager();
			virtual void init();
			virtual void shutdown();
			template<typename T>
			inline bool dispatch(T notifyFn,unsigned int SynchronizationKey = SYNC_DISABLED);
			template<typename T,typename T1>
			inline bool dispatch(T notifyFn,EventResponse<T1> &response,unsigned int SynchronizationKey = SYNC_DISABLED);
	};

	template<typename T>
	inline bool NotifyManager::dispatch(T notifyFn,unsigned int SynchronizationKey)
	{
		if(likely(SynchronizationKey == SYNC_DISABLED))
		{
			return pNotifier[this->m_SyncKey++ % NOTIFIER_COUNT].addEvent(notifyFn);
		}
		return pNotifier[SynchronizationKey % NOTIFIER_COUNT].addEvent(notifyFn);
	}

	template<typename T,typename T1>
        inline bool NotifyManager::dispatch(T notifyFn,EventResponse<T1> &response,unsigned int SynchronizationKey)
        {       
                if(likely(SynchronizationKey == SYNC_DISABLED))
                {       
                        return pNotifier[this->m_SyncKey++ % NOTIFIER_COUNT].addEvent(notifyFn,response);
                }       
                return pNotifier[SynchronizationKey % NOTIFIER_COUNT].addEvent(notifyFn,response);        
	}
}

#endif //NOTIFYMANAGER_H
