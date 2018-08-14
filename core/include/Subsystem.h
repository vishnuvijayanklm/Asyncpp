#ifndef SUBSYSTEM_H_INCLUDED
#define SUBSYSTEM_H_INCLUDED
#include <string>
#include <iostream>
#include <defines.h>
using namespace std;

namespace Core
{
    class ISubsystem
    {
        const string m_name;
    public:
        ISubsystem(string name):m_name(name){}
        virtual void init() = 0;
        virtual void shutdown() = 0;
        const string& name() const
        {
            return this->m_name;
        }

    };
}
#endif // SUBSYSTEM_H_INCLUDED
