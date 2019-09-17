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
				//CallInfo<decltype(args...)> callInfo(args...);				
				auto event = static_cast<std::function<void(Args...)>*>(this->mEvent);
				if(event)
				{
					(*event)(args...);
				}
			}
	};

	class EventListener
        {
                        StlMap<std::string,std::shared_ptr<StlQueue<std::shared_ptr<Event>>>> mEvents;

			const Core::SyncKey mSyncKey;
                public:
                        EventListener():mSyncKey(Core::Synchronizer::getSyncKey())
                        {
				
                        }

                        ~EventListener()
                        {
                        }

			template<typename Fn>
                        void addEvent(std::string eventName,Fn fn)
                        {
                                std::shared_ptr<StlQueue<std::shared_ptr<Event>>> pEventQ;
                                this->mEvents.find(eventName,pEventQ);
                                if(!pEventQ)
                                {
                                        pEventQ = make_shared<StlQueue<std::shared_ptr<Event>>>();
                                        this->mEvents.insert(eventName,pEventQ);
                                }
                                pEventQ->push(make_shared<Event>(fn));
                        }

                        void removeEvent(std::string eventName)
                        {
                                this->mEvents.erase(eventName);
                        }

		
				
			template<typename... Args>
                        void notify_sync(std::string eventName,Args&&... args)
                        {
                                std::shared_ptr<StlQueue<std::shared_ptr<Event>>> pEventQ;
                                shared_ptr<Event> ptr;
                                if(this->mEvents.find(eventName,pEventQ))
                                {
                                        StlQueue<std::shared_ptr<Event>> temp = *pEventQ;
                                        while(temp.pop(ptr))
                                        {
                                                if(ptr)
                                                {
                                                        Async::Task(bind(&Event::triggerEvent<Args&...>,ptr,std::forward<Args>(args)...)).execute_sync(this->mSyncKey);
                                                }
                                        }
                                }
                        }

                        template<typename... Args>
                        void notify_async(std::string eventName,Args&&... args)
                        {
                                std::shared_ptr<StlQueue<std::shared_ptr<Event>>> pEventQ;
                                shared_ptr<Event> ptr;
                                if(this->mEvents.find(eventName,pEventQ))
                                {
                                        StlQueue<std::shared_ptr<Event>> temp = *pEventQ;
                                        while(temp.pop(ptr))
                                        {
                                                if(ptr)
                                                {
							Async::Task(bind(&Event::triggerEvent<Args&...>,ptr,std::forward<Args>(args)...)).execute_async();
                                                }
                                        }
                                }
                        }

			template<typename... Args>
			void notify_once(std::string eventName,Args&&... args)
			{
				std::shared_ptr<StlQueue<std::shared_ptr<Event>>> pEventQ;
				shared_ptr<Event> ptr;
				if(this->mEvents.erase(eventName,pEventQ))
				{
					StlQueue<std::shared_ptr<Event>> temp = *pEventQ;
					while(temp.pop(ptr))
					{
						if(ptr)
						{
							Async::Task(bind(&Event::triggerEvent<Args&...>,ptr,std::forward<Args>(args)...)).execute_async();
						}
					}
				}
			}
	};
}
#endif // EVENETLISTENER_H
