#ifndef TASKINFO_H
#define TASKINFO_H

#include <functional>

class ITaskInfo
{
	public:
	ITaskInfo()
	{
	}

	virtual ~ITaskInfo()
	{
	}

	virtual void executeTask() = 0;
};


template<typename T>
class TaskInfo : public ITaskInfo
{
	T mTask;
	public:
	TaskInfo(T task):mTask(task)
	{
		
	}

	~TaskInfo()
	{
	}

	void executeTask() override
	{
		this->mTask();
	}

	T getTask()
	{
		this->mTask;
	}
};

template<typename T,typename T1>
class TaskInfoResponse : public ITaskInfo
{
	T mTask;
        T1 mResponse;

	public:
	
	TaskInfoResponse(T task,T1 response): mTask(task),mResponse(response)
	{
		
	}

	~TaskInfoResponse()
	{
	}

	void executeTask() override
        {
		this->mResponse(this->mTask());
        }

        T getTask()
        {
                this->mTask;
        }

};
#endif
