#include <async/include/PollManager.h>
#include <iostream>
#include <unistd.h>
using namespace std;
namespace Async
{
	PollManager::PollManager()
	{
		this->open();
		this->start();
	}

	PollManager::~PollManager()
	{
	}

	PollManager* PollManager::getInstance()
	{
		static PollManager *pPollManager = new PollManager();
		return pPollManager;
	}

	void PollManager::open()
	{
		this->mEpollFd = epoll_create1(0);
	}
	
	bool PollManager::addEventListener(int fd, IEventListener *pListener)
	{
		if(pListener != nullptr)
		{
			struct epoll_event event;
			event.events = EPOLLIN | EPOLLRDHUP | EPOLLERR; 
			event.data.ptr = pListener;

			if(epoll_ctl(this->mEpollFd,EPOLL_CTL_ADD,fd,&event) == -1)
			{
				return false;
			}
			this->mListenerMap.insert(fd,pListener);
			return true;
		}
		return false;
	}

	void PollManager::run()
	{
		struct epoll_event *events = new epoll_event[MAXEVENTS];
		uint32_t event;
		int fd,eventCount;

		while(1)
		{
			eventCount = epoll_wait (this->mEpollFd,events,MAXEVENTS,-1);
			 
			if((eventCount > MAXEVENTS) || (eventCount <= 0))
        		{
            			usleep(1000);
            			continue;
        		}

        		for (int i = 0; i < eventCount; i++)
        		{
				IEventListener *pListener = static_cast<IEventListener*>(events[i].data.ptr);
				if(pListener != nullptr)
				{
					pListener->onEventReceived();
				}
			}
		}
	}
}
