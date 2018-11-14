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
	class Notifier:public Util::Runnable
	{
		private:
			StlQueue<unique_ptr<IEventInfo>> m_Queue;
			void initialize();
		protected:

		public:
			Notifier();
			virtual ~Notifier();
			void run() override;
			inline void onStop() override;
			template<typename T>
			inline bool addEvent(T event);
	};

	template<typename T>
	bool Notifier::addEvent(T event)
	{
		this->m_Queue.push(unique_ptr<IEventInfo>(new Event<T>(event)));
		return true;
	}
	void Notifier::onStop()
	{
		this->m_Queue.push(unique_ptr<IEventInfo>(nullptr));
	}	
}
#endif // NOTIFIER_H
