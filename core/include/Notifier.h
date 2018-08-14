#ifndef NOTIFIER_H
#define NOTIFIER_H

#include <Stl.h>
#include <EventInfo.h>
#include <thread>
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
    protected:

    public:
        Notifier();
        virtual ~Notifier();
        void run();
        template<typename T>
        inline bool addEvent(T event);
    };
    template<typename T>
    bool Notifier::addEvent(T event)
    {
        this->m_Queue.push(unique_ptr<IEventInfo>(new Event<T>(event)));
        return true;
    }

}
#endif // NOTIFIER_H
