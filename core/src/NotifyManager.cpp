#include "NotifyManager.h"
namespace Core
{
    NotifyManager::NotifyManager(string name):ISubsystem(name)
    {
        this->pNotifier = nullptr;
    }

    NotifyManager::~NotifyManager()
    {
        //dtor
    }

    void NotifyManager::init()
    {
        LOG_VERBOSE((LOGGER),("Notifier::init>>>"));
#if NOTIFIER_COUNT < 1
#error No notifier avaliable to process
#endif
        this->m_SyncKey = 0;
        this->pNotifier = new Notifier[NOTIFIER_COUNT];
        LOG_VERBOSE((LOGGER),("Notifier::init<<<"));
    }
    void NotifyManager::shutdown()
    {
        LOG_VERBOSE((LOGGER),("Notifier::Shutdown>>>"));

        if(this->pNotifier)
        {
            delete[] this->pNotifier;
        }
        this->pNotifier = nullptr;
        LOG_VERBOSE((LOGGER),("Notifier::Shutdown<<<"));
    }
}
