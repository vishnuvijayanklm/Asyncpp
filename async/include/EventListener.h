#ifndef EVENETLISTENER_H
#define EVENETLISTENER_H

#include <util/include/Runnable.h>
#include <sys/epoll.h>

#define MAXEVENTS 0x40

namespace Async
{
	class IEventListener
	{
		public:
			virtual void onEventReceived() = 0;
	};
}
#endif // EVENETLISTENER_H
