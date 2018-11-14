#ifndef THREAD_H
#define THREAD_H
#include <Stl.h>
#include <Runnable.h>
#include <EventInfo.h>
using namespace std;
namespace Core
{
	class Thread:public Util::Runnable
	{
			StlQueue<unique_ptr<IEventInfo>> m_Queue;	
		public:
        		Thread();
			virtual ~Thread();
			void run() override;
                        void onStop() override;
			template<typename T>
			void execute(T); 
	};
}
#endif
