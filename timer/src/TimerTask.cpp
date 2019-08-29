#include <timer/include/TimerTask.h>
#include <async/include/PollManager.h>
#include <unistd.h>

namespace Async
{
	TimerTask::TimerTask()
	{
		this->initialize();
		this->start();
	}
	
	TimerTask* TimerTask::getTimer()
	{
		static TimerTask *pTimerTask = new TimerTask;
		return pTimerTask;
	}

	TimerTask::~TimerTask()
	{

	}

	void TimerTask::initialize()
	{
		this->mTimerFd = -1;
		this->mTimerMap.setCallBack(bind(&TimerTask::onTimerExpired,this,placeholders::_1,placeholders::_2));
	}

	void TimerTask::start()
	{
		this->mTimerFd = timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK);
		struct itimerspec ts;

		ts.it_interval.tv_sec = 0;;
		ts.it_interval.tv_nsec = 200 * 1000000;
		ts.it_value.tv_sec = 0;
		ts.it_value.tv_nsec = 200 * 1000000;

		if(timerfd_settime(this->mTimerFd, 0, &ts, NULL) == 0)
		{
			Async::PollManager::getInstance()->addEventListener(this->mTimerFd,this);
		}
	}

	void TimerTask::onEventReceived()
	{
		uint64_t message;
		if(read(this->mTimerFd,&message,sizeof(uint64_t)) > 0)
		{
			lock_guard<mutex> lock(this->mMutex);
			this->mTimerMap.houseKeep();
			Core::NotifyManager::getInstance()->printStatus();	
		}
	}
	
	void TimerTask::addTimer(ITimer *pTimer,TimerTicks *pTicks)
	{
		lock_guard<mutex> lock(this->mMutex);
		if(pTimer && pTicks && pTicks->isActive())
		{
			this->mTimerMap.insert(pTicks,pTimer,pTicks->getInterval());
		}
	}

	void TimerTask::onTimerExpired(TimerTicks *pTicks,ITimer *pTimer)
	{
		if(pTimer && pTicks && pTicks->isActive())
		{
			pTicks->onExpired();
			if(pTicks->isRepetitive() && pTicks->isActive())
			{
				this->mTimerMap.insert(pTicks,pTimer,pTicks->getInterval());
			}
			Async::Task().add(std::bind(&ITimer::onTimerExpired,pTimer,pTicks)).setCancellationToken(pTicks->getCancellationToken()).execute_sync(pTimer->getSyncKey());
		}
	}
	
	void TimerTask::removeTimer(TimerTicks *pTicks)
	{
		lock_guard<mutex> lock(this->mMutex);
		if(pTicks)
		{
			this->mTimerMap.erase(pTicks);
		}	
	}
}
