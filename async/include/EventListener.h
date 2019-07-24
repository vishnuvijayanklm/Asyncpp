#ifndef EVENETLISTENER_H
#define EVENETLISTENER_H

#include <util/include/Runnable.h>
#include <sys/epoll.h>
#include <containers/include/Stl.h>
#include <async/include/TaskInfo.h>

#define MAXEVENTS 0x40

namespace Async
{
	class IEventListener
	{
		public:
			virtual void onEventReceived() = 0;
	};

	
	class IEvent
	{
		public:
			IEvent()
			{
			}
	};

	template<typename T>
	class Event : public IEvent
	{
			T& mEvent;
		public:
			Event(T& fn):mEvent(fn)
			{
				fn(100,-200);
			}	
			/*	
			void execute(int x,...) override
			{
			}
			
			template<typename ... Args>
			void exe(Args&&... args)
			{
				cout<<(std::forward<Args...>(args...))<<endl;	
				//this->mEvent(forward<Args...>(args...));
			}*/
	};
	class EventListener
	{
			StlMap<std::string,std::shared_ptr<IEvent>> mEvents;
		public:
			EventListener()
			{
			}
		
			~EventListener()
			{
			}

			template<typename Fn>
			void addEvent(std::string eventName,Fn&& fn)
			{
				this->mEvents.insert(eventName,std::make_shared<Event<Fn>>(fn));	
			}
			
			template<typename... Args>
			void async_notify(std::string eventName,Args&&... args)
			{
				shared_ptr<IEvent> ptr;
				if(this->mEvents.find(eventName,ptr))
				{
					if(ptr)
					{
						//ptr->execute(args...);
					}
				}
			}
	};
}
#endif // EVENETLISTENER_H
