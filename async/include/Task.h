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
			CompletionTokenPtr mCompletionToken;

			template<typename Task>
                        void addTaskToQ(Task task)
                        {
                        	this->mTasks.push(make_shared<TaskInfo<Task>>(task,this->mCompletionToken));
			}

			template<typename Task,typename Response>
                        void addTaskToQ(Task task,Response response)
                        {
				this->mTasks.push(make_shared<TaskInfoResponse<Task,Response>>(task,response,this->mCompletionToken));
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
			ITask(Task task):ITask()
			{
				this->addTaskToQ(task);
			}

			template<typename Task,typename Response>
			ITask(Task task,Response response):ITask()
			{
				this->addTaskToQ(task,response);
			}
			
			template<typename Fn>
			ITask& add(Fn Task)
			{
				this->addTaskToQ(Task);
				return *this;
			}

			template<typename Fn,typename Resp>
			ITask& add(Fn Task,Resp Response)
			{
				this->addTaskToQ(Task,Response);	
				return *this;
			}

			ITask& setCancellationToken(std::shared_ptr<Async::CancellationToken> &cancellationToken)
			{
				this->mCompletionToken->setCancellationToken(cancellationToken);
				return *this;
			}
	
			CompletionTokenPtr& execute_async()
			{
				shared_ptr<ITaskInfo> pTask;
				while(this->mTasks.pop(pTask))
				{
					if(pTask)
					{
						pTask->setCancellationToken();
						Core::NotifyManager::getInstance()->dispatch(pTask,Core::Synchronizer::getSyncKey());		
					}		
				}

				return this->mCompletionToken;
			}

			CompletionTokenPtr& execute_sync(Core::SyncKey syncKey = Core::Synchronizer::getSyncKey())
			{
				shared_ptr<ITaskInfo> pTask;
                                while(this->mTasks.pop(pTask))
                                {
                                        if(pTask)
                                        {
                                                pTask->setCancellationToken();
                                                Core::NotifyManager::getInstance()->dispatch(pTask,syncKey);
                                        }
                                }

				return this->mCompletionToken;
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
