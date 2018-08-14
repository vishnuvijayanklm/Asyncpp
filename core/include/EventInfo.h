#ifndef EVENTINFO_H_INCLUDED
#define EVENTINFO_H_INCLUDED

#include <memory>
class IEventInfo
{
    private:
    protected:
    public:
        IEventInfo() {}
        virtual ~IEventInfo(){}
        virtual void processEvent() = 0;
};

template<typename T>
class Event:public IEventInfo
{
private:
    T m_Event;
public:
    Event(T &event):m_Event(event)
    {
    }
    virtual ~Event(){}
    virtual void processEvent()
    {
        this->m_Event();
       // this->m_Promise->set_value(this->m_Event());
    }
};
#endif // EVENTINFO_H_INCLUDED
