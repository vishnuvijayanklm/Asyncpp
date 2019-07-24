#include <iostream>
#include <util/include/Logger.h>
#include <async/include/EventListener.h>
#include <future>
#include <async/include/Task.h>

using namespace std;
//unique_ptr<Core::Application> pApplication(new Core::Application());
Logger LOGGER;

void test()
{
	cout<<"IN TEST"<<endl;
	sleep(1);	
}
int main()
{
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
                        });*/

	pEvent->addEvent("event1",[](int x,int y)
                        {
                                cout<<"onEvent1"<<endl;
                        });
	pEvent->async_notify("event1",10,11,10,1000);
	return 0;
	//Async::SyncTask(test).add(test);//.add(test);
	while(1)
	{
			Async::SyncTask([]()
			{
				return -100;
			},
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
