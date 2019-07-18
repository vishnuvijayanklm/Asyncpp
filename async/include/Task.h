#ifndef ASYNCTASK_H
#define ASYNCTASK_H
#include <containers/include/Stl.h>
#include <async/include/TaskInfo.h>

namespace Async
{

	class Task
	{
			StlQueue<shared_ptr<ITaskInfo> > mTasks;
		public:
			template<typename T>
			Task(T fn)
			{
				this->mTasks.push(shared_ptr<ITaskInfo>(new TaskInfo<T>(fn)));
			}

			template<typename T,typename T1 = function<void(int)>>
                        Task(T fn,T1 resp)
                        {
				if(resp != nullptr)
				{
					this->mTasks.push(shared_ptr<ITaskInfo>(new TaskInfoResponse<T,T1>(fn,resp)));
                        	}
			}

			~Task()
			{
				this->mTasks.clear();
			}

			template<typename T>
                        Task& then(T fn)
			{
				this->mTasks.push(shared_ptr<ITaskInfo>(new TaskInfo<T>(fn)));
				return *this;
			}

			template<typename T,typename T1 >
			Task& then(T fn,T1 resp)
			{
				if(resp != nullptr)
				{
					this->mTasks.push(shared_ptr<ITaskInfo>(new TaskInfoResponse<T,T1>(fn,resp)));
				}
				return *this;
			}
			Task& execute()
			{
				shared_ptr<ITaskInfo> ptr;
                                while(this->mTasks.pop(ptr))
                                {
                                        if(ptr.get() != nullptr)
                                        {
                                                ptr.get()->executeTask();
                                        }
                                }
				return *this;
			}
	};
}
#endif //ASYNCTASK_H
