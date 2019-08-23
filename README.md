# Async++

Async++ is a framework written in c++ for developing async operations.
Features supported

* Async operation
* Advance Logger
* Event Notifers
* Thread Pool
* Stl and Lock Free Queues
* IPC
* Timers


Examples
========
## AsyncTask
Set of n tasks will be executed asynchronosly/parallel
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
.execute_async();

```
## SyncTask
Set of n tasks will be executed synchronosly
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
.execute_sync();

```

## Cancellable Task
*Executing tasks cannot be cancelled.

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
Token->cancel();
```

