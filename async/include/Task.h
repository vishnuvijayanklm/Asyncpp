#ifndef ASYNCTASK_H
#define ASYNCTASK_H
#include <containers/include/Stl.h>
#include <async/include/TaskInfo.h>
#include <core/include/NotifyManager.h>
#include <core/include/Synchronizer.h>

namespace Async
{

	class ITask
	{

			StlQueue<shared_ptr<ITaskInfo>> mTasks;
			std::shared_ptr<Async::CancellationToken> mCancellationToken;		
	
			template<typename Task>
                        void dispatchTask(Task task)
                        {
                        	this->mTasks.push(make_shared<TaskInfo<Task>>(task));
			}

			template<typename Task,typename Response>
                        void dispatchTask(Task task,Response response)
                        {
				this->mTasks.push(make_shared<TaskInfoResponse<Task,Response>>(task,response));
			}

		public:
			ITask()
			{
			}

			virtual ~ITask()
			{
				
			}

			template<typename Task>
			ITask(Task task)
			{
				this->dispatchTask(task);
			}

			template<typename Task,typename Response>
			ITask(Task task,Response response)
			{
				this->dispatchTask(task,response);
			}
			
			template<typename Fn>
			ITask& add(Fn Task)
			{
				this->dispatchTask(Task);
				return *this;
			}

			template<typename Fn,typename Resp>
			ITask& add(Fn Task,Resp Response)
			{
				this->dispatchTask(Task,Response);	
				return *this;
			}

			ITask& setCancellationToken(const std::shared_ptr<Async::CancellationToken> &cancellationToken)
			{
				this->mCancellationToken = cancellationToken;
				return *this;
			}
	
			virtual Core::SyncKey getKey() = 0;


			void execute()
			{
				shared_ptr<ITaskInfo> pTask;
				while(this->mTasks.pop(pTask))
				{
					if(pTask)
					{
						pTask->setCancellationToken(this->mCancellationToken);
						Core::NotifyManager::getInstance()->dispatch(pTask,getKey());		
					}		
				}
			}
	};

	class SyncTask	: public ITask
	{
			Core::SyncKey mKey;
		public:

			SyncTask(const Core::SyncKey key):mKey(key)
			{

			}
			SyncTask():mKey(Core::Synchronizer::getSyncKey())
                        {
                                
                        } 
			
			template<typename Task>
                        SyncTask(Task task) : mKey(Core::Synchronizer::getSyncKey()),ITask(task)
			{
			
			}

			template<typename Task,typename Response>
                        SyncTask(Task task,Response response) : mKey(Core::Synchronizer::getSyncKey()),ITask(task,response)
                        {

                        }

			Core::SyncKey getKey() override
			{
				return this->mKey;
			}
	};

	class AsyncTask  : public ITask
        {
                public:

                        AsyncTask()
                        {

                        }

                        template<typename Fn>
                       	AsyncTask(Fn Task) : ITask(Task)
                        {

                        }

                        template<typename Fn,typename Resp>
                        AsyncTask(Fn Task,Resp Response): ITask(Task,Response)
                        {

                        }

                        Core::SyncKey getKey() override
                        {
                                return Core::Synchronizer::getSyncKey();;
                        }
        };
}
#endif //ASYNCTASK_H
