#ifndef ASYNCTASK_H
#define ASYNCTASK_H
#include <containers/include/Stl.h>
#include <async/include/TaskInfo.h>
#include <core/include/NotifyManager.h>
#include <core/include/Synchronizer.h>

namespace Async
{

	enum class TASK_EXECUTION_TYPE
	{
		ASYNC,
		SYNC
	};

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

			Task& execute(TASK_EXECUTION_TYPE executionType = TASK_EXECUTION_TYPE::SYNC)
			{
				shared_ptr<ITaskInfo> ptr;
				
				Core::SyncKey Key;

				if(executionType == TASK_EXECUTION_TYPE::SYNC)
                                {
				 	Key = Core::Synchronizer::getSyncKey();
				}
                                while(this->mTasks.pop(ptr))
                                {
                                        if(ptr.get() != nullptr)
                                        {
						if(executionType == TASK_EXECUTION_TYPE::SYNC)
						{
							Core::NotifyManager::getInstance()->dispatch(ptr,Key);
						}
						else
						{
							Core::NotifyManager::getInstance()->dispatch(ptr,Core::Synchronizer::getSyncKey());
                                        	}
					}
                                }
				return *this;
			}
	};

	class ITask
	{
		public:
			template<typename T>
			ITask(T Task)
			{
				this->dispatchTask(Task);
			}

			template<typename T,typename T1>
			ITask(T Task,T1 Response)
			{
				this->dispatchTask(Task,Response);
			}

			template<typename T>
			ITask& then(T Task)
			{
				this->dispatchTask(Task);
				return *this;
			}

			template<typename T,typename T1>
			ITask& then(T Task,T1 Response)
			{
				this->dispatchTask(Task,Response);	
				return *this;
			}

			template<typename T>            
                        void dispatchTask(T Task)
                        {
                                Core::NotifyManager::getInstance()->dispatch(make_shared<TaskInfo<T>>(Task),this->getKey());
                        }

			template<typename T,typename T1>
			void dispatchTask(T Task,T1 Response)
			{
				Core::NotifyManager::getInstance()->dispatch(make_shared<TaskInfoResponse<T,T1>>(Task,Response),this->getKey());
			}
	
			virtual Core::SyncKey getKey() = 0;
	};

	class SyncTask	: public ITask
	{
			const Core::SyncKey mKey;
		public:
			template<typename T>
			SyncTask(T Task):ITask(Task),mKey(Core::Synchronizer::getSyncKey())
			{
				
			}	
			
			template<typename T,typename T1>
			SyncTask(T Task,T1 Response):ITask(Task,Response),mKey(Core::Synchronizer::getSyncKey())
			{
			}

			Core::SyncKey getKey() override
			{
				return this->mKey;
			}
	};
}
#endif //ASYNCTASK_H
