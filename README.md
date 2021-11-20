## Async++

Async++ is a framework written in c++ for developing async operations.
Features supported

* Async operation
* Advance Logger
* Event Notifers
* Thread Pool
* Stl and Lock Free Queues
* IPC
* Timers


## Examples

### Task
```cpp 
Async::Task([]() // Task 1
{
	/* Funtion with return type
	   Return value will be captured in the next block */

	   return -100;
},
[](int x)
{	
	/*Return type captured here */
	// X will be -100
})
.add([]() //Task 2
{
	/*Function without a return type */
})
.execute_async(); /* Tasks will be executed asynchronously/parallel */

Async::Task([]() // Task 1
{
	/* Funtion with return type
	   Return value will be captured in the next block */

	   return -100;
},
[](int x)
{	
	/*Return type captured here */
	// X will be -100
})
.add([]() //Task 2
{
	/*Function without a return type */
})
.execute_sync(); /* Tasks will be executed syncronously */

```

### Cancellable Task

```cpp 
std::shared_ptr<Async::CancellationToken> Token = make_shared<Async::CancellationToken>();
Async::Task([]() // Task 1
{
	/* Funtion with return type
	   Return value will be captured in the next block */

	   return -100;
},
[](int x)
{	
	/*Return type captured here */
	// X will be -100
})
.add([]() //Task 2
{
	/*Function without a return type */
})
.setCancellationToken(Token)
.execute_async();
...
Token->cancel(); /* All the pending tasks will be cancelled, executing tasks cannot be cancelled */
```

### EventListeners
```cpp
Async::EventListener *pEvent = new Async::EventListener();
pEvent->addEvent("event1",[](int x,int y)
                        {
                                cout<<"onEvent1_1"<<endl;
                        });
pEvent->addEvent("event1",[](int x,int y)
                        {
                                cout<<"onEvent1_2"<<endl;
                        });
pEvent->addEvent("event2",[](int x,int y)
			{
                                cout<<"onEvent2"<<endl;
                        });
pEvent->addEvent("event3",[]()
			{
                                cout<<"onEvent3"<<endl;
                        });
pEvent->notify_sync("event1",10,100);
pEvent->notify_async("event2");
pEvent->notify_async("event3");

```
### Timer

```cpp
class TimerExample : public Async::ITimer
{
                shared_ptr<Async::TimerTicks> mTicks;
        public:
                TimerExample():mTicks(make_shared<Async::TimerTicks>(this))
                {
                        this->mTicks->setInterval(1);   //Timer set for 1 sec
                        this->mTicks->start();  	//Starting timer
                }

                ~TimerExample()
                {

                }

                void onTimerExpired(Async::TimerTicks *pTicks)
                {
			//Callback after the timer expiry...
                }

};

```
### IPC Message Queue
```cpp
IPC::MessageQueue myQ(/* Q name */,/* Q size */, /* message size */,/* is to create or not */);

myQ.recv([](shared_ptr<char> ptr,size_t size) //Asynchronous receive
{
});

myQ.read(/* buffer ptr */,/* size */); //Synchronous receive

myQ.send(/* buffer ptr */,/* size */); //Synchronous send
```

### Channels
```cpp
Core::Channel<int> c; // Creating a channel
c << 10; // Send to channel

int x;
x << c; //Receving from channel

c.close(); //Closing the channel
```
