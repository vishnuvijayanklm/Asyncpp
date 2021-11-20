#ifndef POLLMANAGER_H
#define POLLMANAGER_H

#include <util/include/Runnable.h>
#include <async/include/EventListener.h>
#include <containers/include/Stl.h>
#include <sys/epoll.h>

#define MAXEVENTS 0x40

namespace Async
{
	class PollManager : public Util::Runnable
	{
		int mEpollFd;

		StlMap<int, IEventListener *> mListenerMap;

		PollManager();
		void open();
		void close();

	public:
		~PollManager();
		static PollManager *getInstance();
		void run() override;

		bool addEventListener(int fd, IEventListener *pListener);
	};
}
#endif // POLLMANAGER_H
