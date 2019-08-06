#ifndef TASKINFO_H
#define TASKINFO_H

#include <functional>

namespace Async
{

	class CancellationToken
	{
		public:
		CancellationToken()
		{
		}
		~CancellationToken()
		{
		}

		bool isCancellable()
		{
			return true;
		}
	};
	class ITaskInfo
	{
		public:
			ITaskInfo(){}

			virtual ~ITaskInfo(){}
			virtual void executeTask() = 0;
	};


	template<typename Fn>
	class TaskInfo : public ITaskInfo
	{
			Fn mTask;
		public:
			TaskInfo(Fn task):mTask(task)
			{
			}	

			~TaskInfo()
			{
			}

			void executeTask() override
			{
				this->mTask();
			}
	};

	template<typename Fn,typename Resp>
	class TaskInfoResponse : public ITaskInfo
	{
		Fn mTask;
		Resp mResponse;

	public:

		TaskInfoResponse(Fn task,Resp response): mTask(std::forward<Fn>(task)),mResponse(std::forward<Resp>(response))
		{

		}

		~TaskInfoResponse()
		{
		}

		void executeTask() override
		{
			this->mResponse(this->mTask());
		}
	};


	template<typename Fn>
	class TaskInfoCancellable : public ITaskInfo
	{
                        Fn mTask;
			std::weak_ptr<CancellationToken> mToken;
                public:
                        TaskInfoCancellable(Fn task,std::shared_ptr<CancellationToken> token):mTask(task),mToken(token)
                        {
                        }

                        ~TaskInfoCancellable()
                        {
                        }

                        void executeTask() override
                        {
				if(!this->mToken.expired())
				{
                                	this->mTask();
				}
                        }
        };
}
#endif
