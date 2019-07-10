#ifndef EVENTINFO_H
#define EVENTINFO_H

#include <atomic>
#include <thread>

template<typename T>
class EventResponse
{
		T m_Response;
		atomic<bool> m_ResponseAvaliable;
	public:
		EventResponse()
		{
			this->m_ResponseAvaliable.store(false,std::memory_order_relaxed);
		}

		~EventResponse()
		{
		}
		
		T get_value()
		{
			while(!this->m_ResponseAvaliable.load(std::memory_order_relaxed))
			{
				this_thread::yield();
			}
			return this->m_Response;
		}

		void set_value(T response)
		{
			this->m_Response = response;
			this->m_ResponseAvaliable.store(true,std::memory_order_relaxed);
		}
};

class IEventInfo
{
	private:
	protected:
	public:
		IEventInfo() {}
		virtual ~IEventInfo(){}
		virtual void processEvent() = 0;
};

template<typename T>
class Event:public IEventInfo
{
	private:
		T m_Event;
	public:
		Event(T &event):m_Event(event)
		{
		}	
		virtual ~Event(){}
		virtual void processEvent()
		{
			this->m_Event();
		}
};

template<typename T,typename T1>
class EventWithResp:public IEventInfo
{
	private:
		T m_Event;
		EventResponse<T1> &m_Response;
	public:
		EventWithResp(T &event,EventResponse<T1> &response):m_Event(event),m_Response(response)
		{
		}
		virtual ~EventWithResp(){}
		virtual void processEvent()
		{
			this->m_Response.set_value(this->m_Event());
		}
};

#endif // EVENTINFO_H
