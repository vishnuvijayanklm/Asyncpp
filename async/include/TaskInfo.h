#ifndef TASKINFO_H
#define TASKINFO_H

#include <functional>
#include <containers/include/Stl.h>
#include <util/include/defines.h>

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

	make_ptr(Token);

	class CancellationToken
	{
		TokenPtr mToken;

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

		TokenPtr &getToken()
		{
			return this->mToken;
		}

		void cancel()
		{
			this->mToken.reset();
		}
	};

	make_ptr(CancellationToken);

	class CompletionToken
	{
		StlMap<void *, void *> mListeners;
		typedef function<void()> CompletionCallBack;

		StlList<CompletionCallBack> mCallBackList;

		//CompletionCallBack mCompletionCallBack;
		CancellationTokenPtr mCancellationToken;

	public:
		CompletionToken(CompletionCallBack callback = nullptr)
		{
			this->onCompletion(callback);
			this->mCancellationToken = make_shared<CancellationToken>();
		}

		~CompletionToken()
		{
		}

		void setCancellationToken(CancellationTokenPtr &CancellationToken)
		{
			this->mCancellationToken = CancellationToken;
		}

		TokenPtr &getCancellationToken()
		{
			return this->mCancellationToken->getToken();
		}

		bool isCompleted()
		{
			return this->mListeners.empty();
		}

		void addListener(void *pTaskInfo)
		{
			this->mListeners.insert(pTaskInfo, pTaskInfo);
		}

		void removeListener(void *pTaskInfo)
		{
			this->mListeners.erase(pTaskInfo);

			if (this->mListeners.empty())
			{
				CompletionCallBack callback;
				this->mCallBackList.startGet();
				while (this->mCallBackList.getNextElement(callback) && callback)
					callback();
				this->mCallBackList.stopGet();
			}
		}

		void onCompletion(CompletionCallBack callback)
		{
			if (callback != nullptr)
			{
				this->mCallBackList.push_back(callback);
			}
		}

		template <typename... Args>
		void onCompletion(Args... args, CompletionCallBack callback)
		{
			callback();
		}

		template <typename... Args>
		CompletionToken &all(shared_ptr<CompletionToken> &completionToken, Args &&...args)
		{
			cout << "Two arg" << endl;
			return this->all(args...);
		}

		CompletionToken &all(shared_ptr<CompletionToken> &completionToken)
		{
			cout << "One arg" << endl;
			return *this;
		}
	};

	make_ptr(CompletionToken);

	class ITaskInfo
	{
		std::weak_ptr<Async::Token> mCancellationToken;
		CompletionTokenPtr mCompletionTokenPtr;

	public:
		ITaskInfo(CompletionTokenPtr &CompletionToken) : mCompletionTokenPtr(CompletionToken)
		{
			this->mCompletionTokenPtr->addListener(this);
			this->mCancellationToken = this->mCompletionTokenPtr->getCancellationToken();
		}

		virtual ~ITaskInfo() {}
		virtual void executeTask() = 0;

		bool isExpired()
		{
			return this->mCancellationToken.expired();
		}

		void setCancellationToken()
		{
			this->mCancellationToken = this->mCompletionTokenPtr->getCancellationToken();
		}

		void onTaskCompleted()
		{
			this->mCompletionTokenPtr->removeListener(this);
		}
	};

	template <typename Task>
	class TaskInfo : public ITaskInfo
	{
		Task mTask;

	public:
		TaskInfo(Task task, CompletionTokenPtr &CompletionToken) : ITaskInfo(CompletionToken), mTask(task)
		{
		}

		~TaskInfo()
		{
		}

		void executeTask() override
		{
			if (!this->isExpired())
			{
				this->mTask();
			}
			this->onTaskCompleted();
		}
	};

	template <typename Task, typename Response>
	class TaskInfoResponse : public ITaskInfo
	{
		Task mTask;
		Response mResponse;

	public:
		TaskInfoResponse(Task task, Response response, CompletionTokenPtr &CompletionToken) : ITaskInfo(CompletionToken), mTask(std::forward<Task>(task)), mResponse(std::forward<Response>(response))
		{
		}

		~TaskInfoResponse()
		{
		}

		void executeTask() override
		{
			if (!this->isExpired())
			{
				this->mResponse(this->mTask());
			}
			this->onTaskCompleted();
		}
	};

	/*
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
        };*/

	make_ptr(ITaskInfo);
}
#endif
