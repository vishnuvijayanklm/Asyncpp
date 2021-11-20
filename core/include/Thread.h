#ifndef THREAD_H
#define THREAD_H
#include <containers/include/Stl.h>
#include <util/include/Runnable.h>

using namespace std;
namespace Core
{
	class Thread : public Util::Runnable
	{
		//	StlQueue<unique_ptr<IEventInfo>> m_Queue;
	public:
		Thread();
		virtual ~Thread();
		void run() override;
		void onStop() override;
		template <typename T>
		inline void execute(T);
	};
	template <typename T>
	void Thread::execute(T fn)
	{
		//this->m_Queue.push(unique_ptr<IEventInfo>(new Event<T>(fn)));
	}
}
#endif
