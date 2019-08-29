#ifndef NOTIFIER_H
#define NOTIFIER_H

#include <containers/include/Stl.h>
#include <util/include/Runnable.h>
#include <util/include/defines.h>
#include <async/include/TaskInfo.h>
#include <containers/include/LockFree.h>

using namespace std;
namespace Core
{
	class Notifier : public Util::Runnable
	{
		private:
			StlQueue<shared_ptr<Async::ITaskInfo>> mQueue;
			//LockFreeQueue<shared_ptr<Async::ITaskInfo>> mQueue;
			void initialize();
		protected:

		public:
			Notifier();
			virtual ~Notifier();
			void run();
			
			inline bool addTask(shared_ptr<Async::ITaskInfo>);
			size_t getPendingEventsCount()
			{
				return this->mQueue.size();
			}
	};

	bool Notifier::addTask(shared_ptr<Async::ITaskInfo> task)
	{
		this->mQueue.push(task);
	}
}
#endif // NOTIFIER_H
