#include "Runnable.h"
namespace Util
{
    Runnable::Runnable()
    {
        //ctor
        this->misAlive = false;
    }

    Runnable::~Runnable()
    {
	this->stop();
        this->m_thread.detach();
        //terminate(this->m_thread);
    }

    void Runnable::start()
    {
        this->m_thread = thread(&Runnable::run,this);
        this->misAlive = true;
    }
}

