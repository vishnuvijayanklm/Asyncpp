#ifndef EVENETLISTENER_H
#define EVENETLISTENER_H

#include <util/include/Runnable.h>
#include <sys/epoll.h>
#include <containers/include/Stl.h>
#include <async/include/TaskInfo.h>
#include <core/include/Synchronizer.h>

#include<stdarg.h>

#define MAXEVENTS 0x40
#define add_event(eventName,func)					\
		class eventName						\
		{							\
			public:						\
				eventName()				\
				{					\
				}					\
				void notify()				\
				{					\
					return func();			\
				}					\
		}eventName_obj;		

#define notify_event(eventName) \
	eventName_obj.notify()
	   
/*
#define add_event(eventName,function)\
	class eventName			\
	{   				\
		public:			\
					\
		eventName()		\
		{			\
		}			\
					\
		template<typename... Args>\
		void call(Args&&...args)\
		{			\
			function(args...)\
		}			\
	}; 				
*/


namespace Async
{
	class IEventListener
	{
			const Core::SyncKey mKey;
		public:
			IEventListener():mKey(Core::Synchronizer::getSyncKey())
			{

			}

			virtual ~IEventListener()
			{

			}
			virtual void onEventReceived() = 0;

			virtual Core::SyncKey getSyncKey()
			{
				return this->mKey;
			}
	};

	template<typename Fn>
        class Event;
	class EventWrapper
	{
		public:
			EventWrapper()
			{
			}
			virtual ~EventWrapper()
			{

			}

			template<typename T>
			auto callBack(Event<T> mEvent)->decltype(mEvent.getType())
			{
						
			}
	};

	class IEvent
	{
		public:

			void *ptr;

			IEvent()
			{
				ptr = nullptr;
			}

			virtual void execute(IEvent *ptr,...) 
			{
			}

	
			virtual EventWrapper* executep() = 0;		
			//virtual const type_info& type() = 0;
	};

	template<typename Fn>
        class Event : public IEvent
        {
                      	Fn mEvent;
		public:
			Event(Fn fn):mEvent(fn)
                        {
				cout<<this<<endl;
				
			}
			
			virtual void execute(IEvent *ptr,...) 
			{
				cout<<"Derived"<<endl;
				va_list args;
				va_start(args,ptr);
				//this->mEvent(args);
				//this->exe(-100,-1);
				va_end(args);
			}

			/*template<typename... Args>
			void exe(Args&&... arg)
			{
				this->mEvent(arg...);
				cout<<"Hai"<<endl;
				cout<<this<<endl;
			}
			
			EventWrapper* executep() override
			{
				this->mEvent(-999,-1000);
				//this->mEvent(-10,-11);
				return nullptr;
			}
			
			virtual const type_info& type()
			{
				return typeid(this->mEvent);
			}

			void operator=(int x)
			{
				cout<<"In operator"<<endl;
			}
			
			template<typename... Args>
			void agg(Args&&... args)
			{
				cout<<"In argg"<<endl;
			}
			void executep(IEvent *ptr)
			{
				va_list args;
				va_start(args,ptr);		
				this->mEvent(args);
				va_end(args);
			}*/
			virtual EventWrapper* executep()
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
				//function<void(...)> fn1 = fn;
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
						//ptr->exe(args...);
						//ptr->exe(args...);
						//ptr->execute(ptr.get(),args...);
					}
				}
			}
	};
}
#endif // EVENETLISTENER_H
