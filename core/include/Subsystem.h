#ifndef SUBSYSTEM_H
#define SUBSYSTEM_H
#include <string>
#include <iostream>
#include <core/include/SubsystemList.h>
using namespace std;
namespace Core
{
    class ISubsystem
    {
        const SubsystemId m_id;
	
    public:
        ISubsystem(SubsystemId id):m_id(id){}
        virtual void init() = 0;
        virtual void shutdown() = 0;
        const SubsystemId& id() const
        {
            return this->m_id;
        }

    };
}
#endif // SUBSYSTEM_H
