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

void onExit(void)
{
	cout<<"On Exit"<<endl;
	Core::NotifyManager::getInstance()->onExit();
}

int test()
{
	return rand() % 1000;
}
class TimerExample : public Async::ITimer
{
		shared_ptr<Async::TimerTicks> mTicks;
	public:
		TimerExample():mTicks(make_shared<Async::TimerTicks>(this))
		{
			this->mTicks->setInterval(1,true);
			this->mTicks->start();
		}

		~TimerExample()
		{

		}

		void onTimerExpired(Async::TimerTicks *pTicks)
		{
			LOG_INFO((LOGGER),("Timer Expired [%p]",pTicks));
			//Async::TimerTask::getTimer()->removeTimer(this->mTicks.get());
		}

};

void fn()
{
	int x = rand() % 1000;
	Async::CancellationTokenPtr Token = make_shared<Async::CancellationToken>();
	Async::CompletionTokenPtr completionToken = Async::Task([]()
		{
			srand(time(0));
			return rand() % 100;
		},
		[](int x)
		{
			LOG_INFONP((LOGGER),("Return Value %d",x));
		})
		.setCancellationToken(Token)
		.execute_sync();

	completionToken->onCompletion([&x]
			{
				LOG_INFONP((LOGGER),("Processing Completed Value %d",x));
			});
	if(x % 2)
	{
		Token->cancel();
	}

}
int main()
{
	atexit(onExit);

	LOGGER.setLogFile("Logs","log.txt");
	LOGGER.setLoglevel(63);

	while(1)
        {
                fn();
                usleep(100000);
        	break;
	}

	
	Async::EventListener *pEvent = new Async::EventListener();

	pEvent->addEvent("event1",[]()
                        {
				LOG_INFONP((LOGGER),("Event1_1 Called"));
                        });
        pEvent->addEvent("event1",[]()
                        {
				LOG_INFONP((LOGGER),("Event1_2 Called"));
                        });
        pEvent->addEvent("event1",[]()
                        {
				LOG_INFONP((LOGGER),("Event1_2 Called"));
                        });
        pEvent->addEvent("event1",[]()
                        {
				LOG_INFONP((LOGGER),("Event1_3 Called"));
                        });
	
	pEvent->addEvent("event2",[](int x,int y)
			{
				LOG_INFONP((LOGGER),("Event2 Called [%d,%d]",x,y));
                        });
	pEvent->addEvent("event3",[]()
			{
				LOG_INFONP((LOGGER),("Event3 Called"));
                        });
	pEvent->addEvent("event4",[](int x,int y)
                        {
				LOG_INFONP((LOGGER),("Event4 Called [%d,%d]",x,y));
          		});
	
	
		pEvent->notify_async("event1");
		pEvent->notify_async("event2",rand() % 100,rand() % 100);
		pEvent->notify_async("event3",-100,-110);
		pEvent->notify_sync("event4",rand() % 100,rand() % 100);
		pEvent->notify_sync("event4",-978,-679,-111);
		pEvent->notify_async("event5",rand() % 100,rand() % 100);
		pEvent->notify_async("event6",rand() % 100,rand() % 100);
	/*IPC::MessageQueue myQueue("/22",200,100,true);
	myQueue.open();

	TimerExample e[10000];
	myQueue.recv([](shared_ptr<char> ptr,size_t size)
        {
		ptr.get()[size] = '\0';
                LOG_INFONP((LOGGER),("Received [%s][%d]",ptr.get(),size));
        });

	int i = 0;
	while(1)
	{
		pEvent->notify_async("event1");
		pEvent->notify_async("event2",rand() % 100,rand() % 100);
		pEvent->notify_async("event3",-100,-110);
		pEvent->notify_async("event4",rand() % 100,rand() % 100);
		pEvent->notify_async("event5",rand() % 100,rand() % 100);
		pEvent->notify_async("event6",rand() % 100,rand() % 100);

		string hai = "Hai Vishnu_"+to_string(++i);
		LOG_INFONP((LOGGER),("Send %s Len %d Success %d Errno %d",hai.c_str(),hai.length(),myQueue.send((char*)hai.c_str(),hai.length()),errno));
		
		std::shared_ptr<Async::CancellationToken> Token = make_shared<Async::CancellationToken>();
		Async::Task([]()
			{
				return rand() % 1000;
			},
			[](int x)
			{
				LOG_INFONP((LOGGER),("Got %d",x)); 
			})
			.add([]()
			{
				LOG_INFONP((LOGGER),("Function with no return"));
			})
			.add(test,
			[](int x)
			{
				LOG_INFONP((LOGGER),("Return from test %d",x));
			})
			.setCancellationToken(Token)
			.execute_sync();

			Async::Task([]()
                        {
                                return (rand() % 1000) * -1;
                        },
                        [](int x)
                        {
				LOG_INFONP((LOGGER),("Got %d",x));
                        })
			.add([]()
			{	
				LOG_INFONP((LOGGER),("Function with no return"));
			})
			.add(test,
			[](int x)
                        {
                                LOG_INFONP((LOGGER),("Return from test %d",x));
                        })
			.setCancellationToken(Token)
			.execute_sync();

			
			if(i % 2)
			{
				Token->cancel();
			}
			
			usleep(1000000);
			//Token->cancel();
			//usleep(10000000);	
	}
*/

	return 0;
}
