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
	   Return type will be caputed in the next block */

	   return -100;
},
[](int x)
{	
	/*Return type captured here */
})
.add([]()
{
	/*Fn without a return type */
});
```
