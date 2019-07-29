#ifndef EVENETLISTENER_H
#define EVENETLISTENER_H

#include <util/include/Runnable.h>
#include <sys/epoll.h>
#include <containers/include/Stl.h>
#include <async/include/TaskInfo.h>
#include<stdarg.h>

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
			virtual void execute(IEvent *ptr,...) 
			{
			}
	};

	template<typename T>
        class Event : public IEvent
        {
                        T mEvent;
		public:
			Event(T fn):mEvent(fn)
                        {	
			}
			
			virtual void execute(IEvent *ptr,...) 
			{
				cout<<"Derived"<<endl;
				va_list args;
				va_start(args,ptr);
				this->exe(args);
				va_end(args);
			}

			template<typename... Args>
			void exe(Args&&... arg)
			{
				cout<<"IN EXE "<<forward<Args...>(arg...)<<endl;
				//this->mEvent(arg...);
				cout<<typeid(arg...).name()<<endl;
				//this->test(arg...);
			}
		
			void test(int x,int y)
			{
				
			}
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
						ptr->execute(ptr.get(),args...);
					}
				}
			}
	};
}
#endif // EVENETLISTENER_H
