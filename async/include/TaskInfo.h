#ifndef TASKINFO_H
#define TASKINFO_H

#include <functional>

namespace Async
{
	class Token
	{
		public:
			Token()
			{
			}
			~Token()
			{
			}
	};
	
	class CancellationToken
	{
			std::shared_ptr<Async::Token> mToken;		
		public:

			CancellationToken() : mToken(make_shared<Async::Token>())
			{
			
			}
			~CancellationToken()
			{
			}

			
			bool isCancellable()
			{
				return this->mToken.get() != nullptr;
			}

			bool isValid()
			{
				return this->mToken.get() != nullptr;
			}

			std::shared_ptr<Async::Token>& getToken()
			{
				return this->mToken;
			}

			void cancel()
			{
				this->mToken.reset();
			}	
	};


	class ITaskInfo
	{
			std::weak_ptr<Async::Token> mToken;
			std::shared_ptr<Async::CancellationToken> mCancellationToken;
		public:
			ITaskInfo()
			{
				this->mCancellationToken = make_shared<Async::CancellationToken>();
				this->mToken = this->mCancellationToken->getToken();
			}

			virtual ~ITaskInfo(){}
			virtual void executeTask() = 0;

			bool isExpired()
			{
				return this->mToken.expired();
			}

			void setCancellationToken(const std::shared_ptr<Async::CancellationToken> &cancellationToken)
			{
				if(cancellationToken.get())
				{
					this->mToken = cancellationToken->getToken();
				}
			}
	};


	template<typename Task>
	class TaskInfo : public ITaskInfo
	{
			Task mTask;
						
		public:
			TaskInfo(Task task):mTask(task)
			{
			}	

			~TaskInfo()
			{
			}

			void executeTask() override
			{
				if(!this->isExpired())
				{
					this->mTask();
				}
			}
	};


	template<typename Task,typename Response >
	class TaskInfoResponse : public ITaskInfo
	{
		Task mTask;
		Response mResponse;

	public:

		TaskInfoResponse(Task task,Response response): mTask(std::forward<Task>(task)),mResponse(std::forward<Response>(response))
		{

		}

		~TaskInfoResponse()
		{
		}

		void executeTask() override
		{
			if(!this->isExpired())
                        {
				this->mResponse(this->mTask());
			}
		}
	};


	template<typename Task>
        class TaskInfoAck : public ITaskInfo
        {
                Task mTask;
                std::function<void()> mResponse;
        public:
                TaskInfoAck(Task task,std::function<void()> response): mTask(std::forward<Task>(task)),mResponse(response)
                {

                }

                ~TaskInfoAck()
                {
                }

                void executeTask() override
                {
                        this->mTask();
			this->mResponse();
                }
        };
}
#endif
