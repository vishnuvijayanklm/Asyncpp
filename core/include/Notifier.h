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
	};

	bool Notifier::addTask(shared_ptr<Async::ITaskInfo> task)
	{
		this->m_Queue.push(task);
	}
}
#endif // NOTIFIER_H
