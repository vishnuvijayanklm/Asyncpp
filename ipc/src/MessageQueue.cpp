#include <ipc/include/MessageQueue.h>

namespace IPC
{
	MessageQueue::MessageQueue(key_t key)
	{
		this->mKey = key;
		this->mMsqId = -1;
	}

	MessageQueue::~MessageQueue()
	{
		this->close();
	}

	void MessageQueue::open()
	{
		this->mMsqId = msgget(this->mKey,S_IRUSR | S_IWUSR | IPC_CREAT);		

		if(this->mMsqId == IPC_ERROR)
		{
			throw MESSAGE_QUEUE_OPEN_FAILED();	
		}	
	}	

	void MessageQueue::close()
	{
		if(this->mMsqId != IPC_ERROR)
		{
			msgctl(this->mMsqId,IPC_RMID,NULL); 
		}
	}

	size_t MessageQueue::send(char *ptr,size_t len)
	{
		if(msgsnd(this->mMsqId,static_cast<void*>(ptr),len,0) == IPC_ERROR)
		{
			return IPC_ERROR;
		}
		return len;
	}

	size_t MessageQueue::read(char *ptr,size_t len,bool isblocked)
	{
		int msgflg = 0;
		
		if(!isblocked)
		{
			msgflg |= IPC_NOWAIT;
		}
		return msgrcv(this->mMsqId,ptr,len,0,msgflg);
	}
}
