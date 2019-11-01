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
			CompletionTokenPtr mCompletionToken;

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
				this->mCompletionToken = make_shared<CompletionToken>();
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
	
			void execute_async()
			{
				shared_ptr<ITaskInfo> pTask;
				while(this->mTasks.pop(pTask))
				{
					if(pTask)
					{
						pTask->setCancellationToken(this->mCancellationToken);
						Core::NotifyManager::getInstance()->dispatch(pTask,Core::Synchronizer::getSyncKey());		
					}		
				}
			}

			void execute_sync(Core::SyncKey syncKey = Core::Synchronizer::getSyncKey())
			{
				shared_ptr<ITaskInfo> pTask;
                                while(this->mTasks.pop(pTask))
                                {
                                        if(pTask)
                                        {
                                                pTask->setCancellationToken(this->mCancellationToken);
                                                Core::NotifyManager::getInstance()->dispatch(pTask,syncKey);
                                        }
                                }
			}
	};

	class Task  : public ITask
        {
                public:

                        Task()
                        {

                        }

                        template<typename Fn>
                       	Task(Fn Task) : ITask(Task)
                        {

                        }

                        template<typename Fn,typename Resp>
                        Task(Fn Task,Resp Response): ITask(Task,Response)
                        {

                        }
        };
}
#endif //ASYNCTASK_H
