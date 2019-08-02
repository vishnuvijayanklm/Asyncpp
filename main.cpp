#include <iostream>
#include <util/include/Logger.h>
#include <async/include/EventListener.h>
#include <future>
#include <async/include/Task.h>
#include <timer/include/TimerTask.h>

using namespace std;
//unique_ptr<Core::Application> pApplication(new Core::Application());
Logger LOGGER;

void test()
{
	cout<<"IN TEST"<<endl;
	sleep(1);	
}

void fn(int x)
{
	cout<<"Expired Func"<<endl;
}

class Example : public Async::ITimer
{
	shared_ptr<Async::TimerTicks> mTicks;
	public:
		Example()
		{
			this->mTicks = make_shared<Async::TimerTicks>();
			this->mTicks->setInterval(1,1);
			Async::TimerTask::getTimer()->addTimer(this,this->mTicks);
		}
		void onTimerExpired(Async::TimerTicks *pTicks)
		{
			LOG_INFO((LOGGER),("Timer Expired"));
			//Async::TimerTask::getTimer()->removeTimer(this->mTicks.get());
		}

};
int main()
{
	LOGGER.setLogFile("Logs","log.txt");
	LOGGER.setLoglevel(63);
	Async::EventListener *pEvent = new Async::EventListener();
	
	/*pEvent->addEvent("event1",[]()
			{
				cout<<"onEvent1"<<endl;
			});		
	
	pEvent->addEvent("event2",[]()
			{
                                cout<<"onEvent2"<<endl;
                        });
	pEvent->addEvent("event3",[]()
			{
                                cout<<"onEvent3"<<endl;
                        });
	pEvent->addEvent("event4",[](int x,int y)
			{
                                cout<<"onEvent4"<<endl;
                        });
	pEvent->addEvent("event5",[]()
			{
                                cout<<"onEvent5"<<endl;
                        });
*/
	pEvent->addEvent("event6",[](int x,int y)
                        {
                                cout<<"X = "<<x<<" Y = "<<y<<endl;
          		});
	//add_event(EVENT_1,[](){cout<<"Hai"<<endl;});
	//notify_event(EVENT_1);
	//pEvent->async_notify("event6",-100,-110);
	//pEvent->async_notify("event1");

	//return 0;
 	while(1)
	{
		Example e[20];
		sleep(2);
		break;
	}
	while(1)
	{
		sleep(10);
	}
	return 0;
	//Async::SyncTask(test).add(test);//.add(test);
	while(1)
	{
			Async::SyncTask(bind([]()
			{
				return 99;
			}),
			[](int x)
			{ 
				cout<<"Got "<<x<<endl;
			}).add([]() {cout<<"Fn with no return"<<endl; }).add(&test);
			
			Async::AsyncTask([]()
                        {
                                return -100;
                        },
                        [](int x)
                        {
                                cout<<"Got "<<x<<endl;
                        }).add([]() {cout<<"Fn with no return"<<endl; }).add(test);
			sleep(1);
	}
	return 0;
}
