#ifndef RUNNABLE_H
#define RUNNABLE_H
#include <thread>
#include <functional>
using namespace std;
namespace Util
{
	class Runnable:public thread
	{
		private:
			thread m_thread;
			bool misAlive;	
		protected:

			bool isAlive()
			{
				return this->misAlive;
			}

		public:
			Runnable();

			template<typename Callable, typename... Args>
			explicit Runnable(Callable&& f,Args&&... args)
			{
				this->m_thread = thread(bind(f,forward<Args>(args)...));
			}

			virtual ~Runnable();
			void join()
			{
				this->m_thread.join();
			}

			virtual void run() = 0;
			virtual void onStop()
			{
			}
			void start();
			void stop()
			{
				this->misAlive = false;
				this->onStop();
				this->join();
			}

			void runFn()
			{
				this->misAlive = true;
				this->run();
				this->misAlive = false;
			}
	};
}
#endif // RUNNABLE_H
