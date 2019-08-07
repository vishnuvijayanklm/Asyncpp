#ifndef EVENETLISTENER_H
#define EVENETLISTENER_H

#include <util/include/Runnable.h>
#include <sys/epoll.h>
#include <containers/include/Stl.h>
#include <core/include/Synchronizer.h>
#include <async/include/Task.h>

#include<stdarg.h>

#define MAXEVENTS 0x40


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
	
	template <typename Fn>
	struct Traits : public Traits<decltype(&Fn::operator())>
	{
	};



	template <typename ClassType,typename Return, typename... Args>
	struct Traits<Return(ClassType::*)(Args...) const> 
	{
		typedef std::function<Return(Args...)> Fn;
	};

	class Event
	{
			void *mEvent;
		public:
			template<typename Fn>
			Event(Fn fn)
			{
				auto f = this->getType(fn);
				this->mEvent = new decltype(f)(this->getType(fn));
			}

			template <typename Fn>
			typename Traits<Fn>::Fn getType(Fn fn)
			{
				return static_cast<typename Traits<Fn>::Fn>(fn);
			}

			template<typename...Args>
			void triggerEvent(Args... args)
			{
				auto event = static_cast<std::function<void(Args...)>*>(this->mEvent);
				if(event)
				{
					(*event)(args...);
				}
			}
	};

	class EventListener : public IEventListener
	{
			StlMap<std::string,std::shared_ptr<Event>> mEvents;
		public:
			EventListener()
			{
			}
		
			~EventListener()
			{
			}

			template<typename Fn>
			void addEvent(std::string eventName,Fn fn)
			{
				this->mEvents.insert(eventName,make_shared<Event>(fn));
			}	
			
			template<typename... Args>
			void notify(std::string eventName,Args&&... args)
			{
				shared_ptr<Event> ptr;
				if(this->mEvents.find(eventName,ptr))
				{
					if(ptr)
					{
						//Async::AsyncTask(std::bind(&Event::triggerEvent,ptr,std::forward<Args>(args)...));
						ptr->triggerEvent(args...);
					}
				}
			}
			
			void onEventReceived() override
			{
				
			}
	};
}
#endif // EVENETLISTENER_H
