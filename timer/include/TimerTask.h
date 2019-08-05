#ifndef TIMERTASK_H
#define TIMERTASK_H

#include <async/include/EventListener.h>
#include <async/include/Task.h>
#include <sys/timerfd.h>

namespace Async
{
	class TimerTicks;

	class ITimer : public IEventListener
	{
		public:
			ITimer()
			{
			}
			virtual ~ITimer()
			{
			}
			virtual void onTimerExpired(Async::TimerTicks*) = 0;

			virtual void onEventReceived() {}
	};


	class TimerTask : Async::IEventListener
	{
			int mTimerFd;
			uint64_t mTicksExpired; 

			void initialize();
			void start();

			std::mutex mMutex;

			StlTimedMap<TimerTicks*,ITimer*> mTimerMap;
			
			TimerTask();
			~TimerTask();
		public:
		
			static TimerTask* getTimer();	
			void onEventReceived() override;

			void addTimer(ITimer*,TimerTicks*);
			void removeTimer(TimerTicks*);

			void onTimerExpired(TimerTicks*,ITimer*);
	};
	
	class TimerTicks
	{
			uint64_t mInterval;
			bool mIsRepetitive;		
			bool mIsRunning;
			Async::ITimer *pmTimer;
			std::mutex mMutex;
		public:
			TimerTicks(ITimer *pTimer)
			{
				this->mInterval = 0;
				this->mIsRepetitive = 0;
				this->mIsRunning = false;
				this->pmTimer = pTimer;
			}
			~TimerTicks()
			{
				this->stop();
			}

			void setInterval(uint64_t interval,bool isRepetitive = false)
			{
				this->mInterval = interval;
				this->mIsRepetitive = isRepetitive;
			}

			uint64_t getInterval() const
			{
				return this->mInterval;
			}

			bool isActive() const
			{
				return this->mIsRunning == true;
			}

			bool isRepetitive() const
			{
				return this->mIsRepetitive;
			}

			void start()
			{
				if(this->pmTimer)
				{
					this->mIsRunning = true;
					Async::TimerTask::getTimer()->addTimer(this->pmTimer,this);
				}
			}

			void stop()
			{
				this->mIsRunning = false;
				this->mIsRepetitive = false;
				Async::TimerTask::getTimer()->removeTimer(this);
			}

			void onExpired()
			{
				this->mIsRunning &= this->mIsRepetitive;
			}
	};

}
#endif
