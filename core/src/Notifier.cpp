#include "Notifier.h"

namespace BHI
{
    namespace Core
    {
        Notifier::Notifier()
        {
            this->start();
        }

        Notifier::~Notifier()
        {
            //dtor
        }

        void Notifier::run()
        {
            while(1)
            {
                try
                {
                    LOG_INFO((LOGGER),("Notifier::run>>>"));
                    unique_ptr<IEventInfo> pEvent;
                    while(this->m_Queue.pop(pEvent,true))
                    {
                        if(pEvent.get())
                        {
                            pEvent->processEvent();
                        }
                    }
                }
                catch(Exception &e)
                {

                }
                catch(exception &e)
                {

                }
                catch(...)
                {

                }
            }

        }
    }
}
