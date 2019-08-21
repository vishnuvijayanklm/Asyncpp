#include <iostream>
#include <util/include/Logger.h>
#include <async/include/EventListener.h>
#include <future>
#include <async/include/Task.h>
#include <timer/include/TimerTask.h>
#include <ipc/include/MessageQueue.h>

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
		Example():mTicks(make_shared<Async::TimerTicks>(this))
		{
			this->mTicks->setInterval(1,1);
			this->mTicks->start();
		}

		~Example()
		{

		}

		void onTimerExpired(Async::TimerTicks *pTicks)
		{
			LOG_INFO((LOGGER),("Timer Expired [%p]",pTicks));
			//Async::TimerTask::getTimer()->removeTimer(this->mTicks.get());
		}

};

int main()
{
	/*func([]()
	{
		cout<<"Called...."<<endl;
		return 100;
	},
	[](int x)
        {
                cout<<"Received "<<x<<endl;
        });

	return 0;
*/
	/*,
	[](int x)
	{
		cout<<"Received "<<x<<endl;
	});*/
	LOGGER.setLogFile("Logs","log.txt");
	LOGGER.setLoglevel(63);
	Async::EventListener *pEvent = new Async::EventListener();

	pEvent->addEvent("event1",[]()
                        {
                                cout<<"onEvent1_1"<<endl;
                        });
        pEvent->addEvent("event1",[]()
                        {
                                cout<<"onEvent1_2"<<endl;
                        });
        pEvent->addEvent("event1",[]()
                        {
                                cout<<"onEvent1_3"<<endl;
                        });
        pEvent->addEvent("event1",[]()
                        {
                                cout<<"onEvent1_4"<<endl;
                        });
	
	pEvent->addEvent("event4",[](int x,int y)
			{
                                cout<<"onEvent4"<<endl;
                        });
	pEvent->addEvent("event5",[]()
			{
                                cout<<"onEvent5"<<endl;
                        });
	pEvent->addEvent("event6",[](int x,int y)
                        {
                                cout<<"Event6 X = "<<x<<" Y = "<<y<<endl;
          		});
	//add_event(EVENT_1,[](){cout<<"Hai"<<endl;});
	//notify_event(EVENT_1);
	pEvent->notify("event1",-100,-110);
	pEvent->notify("event2",-100,-110);
	pEvent->notify("event3",-100,-110);
	pEvent->notify("event4",-100,-110);
	pEvent->notify("event5",-100,-110);
	pEvent->notify("event6",-123,-111);
	//pEvent->async_notify("event1");


	IPC::MessageQueue myQueue("/myQ",200,100,true);
	cout<<myQueue.open()<<endl;
	myQueue.recv([](shared_ptr<char> ptr,size_t size)
	{
		LOG_INFONP((LOGGER),("Received [%s][%d]",ptr.get(),size));
	});


	while(1)
	{
		string hai = "Hai Vishnu";
		LOG_INFONP((LOGGER),("Send %d ",myQueue.send((char*)hai.c_str(),hai.length())));
		usleep(100000);
	}
	/*
	while(1)
	{
		char ptr[10];
		LOG_INFONP((LOGGER),("Send [%d]",myQueue.send(ptr,sizeof(int))));
		sleep(1);
	}
	return 0;
	Example e[2000];*/
	
	while(1)
	{
			std::shared_ptr<Async::CancellationToken> Token = make_shared<Async::CancellationToken>();
			Async::SyncTask([]()
			{
				return 99;
			},
			[](int x)
			{ 
				cout<<"Got "<<x<<endl;
			})
			.add([]()
			{
				cout<<"Fn with no return"<<endl;
			})
			.add(&test)
			.setCancellationToken(Token)
			.execute();
			
			Async::AsyncTask([]()
                        {
                                return -100;
                        },
                        [](int x)
                        {
                                cout<<"Got "<<x<<endl;
                        })
			.add([]()
			{
				cout<<"Fn with no return"<<endl;
			})
			.add(test)
			.setCancellationToken(Token)
			.execute();

			Token->cancel();
			Example e[20000];
			sleep(1);
	}
	return 0;
}
