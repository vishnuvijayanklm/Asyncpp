#ifndef NOTIFIER_H
#define NOTIFIER_H

#include <Stl.h>
#include <EventInfo.h>
#include <Exception.h>
#include <Runnable.h>
#include <defines.h>
using namespace std;
namespace Core
{
	class Notifier
	{
		private:
			StlQueue<unique_ptr<IEventInfo>> m_Queue;
			void initialize();
		protected:

		public:
			Notifier();
			virtual ~Notifier();
			void run();
			template<typename T>
			inline bool addEvent(T event);
			template<typename T,typename T1>
			inline bool addEvent(T event,promise<T1> &response);
	};

	template<typename T>
	bool Notifier::addEvent(T event)
	{
		this->m_Queue.push(unique_ptr<IEventInfo>(new Event<T>(event)));
		return true;
	}

	template<typename T,typename T1>
        bool Notifier::addEvent(T event,promise<T1> &response)
        {
                this->m_Queue.push(unique_ptr<IEventInfo>(new EventWithResp<T,T1>(event,response)));
                return true;
        }

	/*
	void Notifier::onStop()
	{
		this->m_Queue.push(unique_ptr<IEventInfo>(nullptr));
	}*/	
}
#endif // NOTIFIER_H
