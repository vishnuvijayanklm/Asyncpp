#ifndef NOTIFYMANAGER_H
#define NOTIFYMANAGER_H

#include <core/include/Subsystem.h>
#include <util/include/defines.h>
#include <core/include/Notifier.h>
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
			
			#ifdef WITH_EVENTPOOL	
			StlQueue<IEventInfo*> mEventPoolQ;
			#endif

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
			#ifdef WITH_EVENTPOOL
			if(this->mEventPoolQ.pop())
			{
			#endif 
				if(likely(SynchronizationKey == SYNC_DISABLED))
				{
					return pNotifier[this->m_SyncKey++ % NOTIFIER_COUNT].addEvent(notifyFn);
				}
				return pNotifier[SynchronizationKey % NOTIFIER_COUNT].addEvent(notifyFn);
			#ifdef WITH_EVENTPOOL
			}
			throw EVENT_POOL_OUTAGE();
			#endif
		}

		template<typename T,typename T1>
		inline bool NotifyManager::dispatch(T notifyFn,EventResponse<T1> &response,unsigned int SynchronizationKey)
		{      
			#ifdef WITH_EVENTPOOL
			if(this->mEventPoolQ.pop())
			{
			#endif
				if(likely(SynchronizationKey == SYNC_DISABLED))
				{       
					return pNotifier[this->m_SyncKey++ % NOTIFIER_COUNT].addEvent(notifyFn,response);
				}       
				return pNotifier[SynchronizationKey % NOTIFIER_COUNT].addEvent(notifyFn,response);        
			#ifdef WITH_EVENTPOOL
			}
			throw EVENT_POOL_OUTAGE();
			#endif
		}
}

#endif //NOTIFYMANAGER_H
