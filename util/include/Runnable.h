#ifndef RUNNABLE_H
#define RUNNABLE_H
#include <thread>
#include <functional>
using namespace std;
namespace BHI
{
    namespace Util
    {
        class Runnable:public thread
        {
            private:
                thread m_thread;

            protected:

            bool isAlive();

            public:
            Runnable();

            template<typename Callable, typename... Args>
            explicit Runnable(Callable&& f,Args&&... args)
            {
                this->m_thread = thread(bind(f,forward<Args>(args)...));
            }

            virtual ~Runnable();
            void join();

            virtual void run() = 0;
            void start();
        };
    }
}
#endif // RUNNABLE_H
