# Async++

Async++ is a framework written in c++ for developing async operations.
Features supported

* Async operation
* Advance Logger
* Event Notifers
* Thread Pool
* Stl and Lock Free Queues
* IPC


Examples
========
```cpp
Async::AsyncTask([]()
{
	/* Funtion with return type
	   Return value will be captured n the next block */

	   return -100;
},
[](int x)
{	
	/*Return type captured here */
	// X will be -100
})
.add([]()
{
	/*Function without a return type */
});
```
