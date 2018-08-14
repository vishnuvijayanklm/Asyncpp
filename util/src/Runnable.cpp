#include "Runnable.h"
namespace BHI
{
    namespace Util
    {
        Runnable::Runnable()
        {
            //ctor
        }

        Runnable::~Runnable()
        {
            //dtor
            this->m_thread.detach();
            //terminate(this->m_thread);
        }

        void Runnable::start()
        {
            this->m_thread = thread(&Runnable::run,this);
        }
    }
}

