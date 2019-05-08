#ifndef EVENTINFO_H
#define EVENTINFO_H

#include <memory>
#include <future>
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
		promise<T1> &m_Response;
	public:
		EventWithResp(T &event,promise<T1> &response):m_Event(event),m_Response(response)
		{
		}
		virtual ~EventWithResp(){}
		virtual void processEvent()
		{
			cout<<"Before this>>>"<<endl;
			T1 resp = this->m_Event();
			cout<<"After this<<<"<<endl;
			this->m_Response.set_value(10);
			//this->m_Response.get_future() = this->m_Event();
			//this->m_Response.set_value(this->m_Event());
		}
};

#endif // EVENTINFO_H
