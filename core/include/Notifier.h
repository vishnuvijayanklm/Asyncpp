#ifndef NOTIFIER_H
#define NOTIFIER_H

#include <containers/include/Stl.h>
#include <core/include/EventInfo.h>
#include <core/include/Exception.h>
#include <util/include/Runnable.h>
#include <util/include/defines.h>
#include <async/include/TaskInfo.h>

using namespace std;
namespace Core
{
	class Notifier : public Util::Runnable
	{
		private:
			StlQueue<shared_ptr<Async::ITaskInfo>> m_Queue;
			void initialize();
		protected:

		public:
			Notifier();
			virtual ~Notifier();
			void run();
			
			inline bool addTask(shared_ptr<Async::ITaskInfo>);
			/*
			template<typename T>
			inline bool addEvent(T event);
			template<typename T,typename T1>
			inline bool addEvent(T event,EventResponse<T1> &response);
			*/
	};

	bool Notifier::addTask(shared_ptr<Async::ITaskInfo> task)
	{
		this->m_Queue.push(task);
	}
	/*
	template<typename T>
	bool Notifier::addEvent(T event)
	{
		this->m_Queue.push(unique_ptr<IEventInfo>(new Event<T>(event)));
		return true;
	}

	template<typename T,typename T1>
        bool Notifier::addEvent(T event,EventResponse<T1> &response)
        {
                this->m_Queue.push(unique_ptr<IEventInfo>(new EventWithResp<T,T1>(event,response)));
                return true;
        }*/
}
#endif // NOTIFIER_H
