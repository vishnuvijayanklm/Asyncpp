#include <ipc/include/MessageQueue.h>
#include <mqueue.h>

namespace IPC
{
	MessageQueue::MessageQueue(string name,size_t maxSize,size_t msgSize,bool isCreate)
	{
		this->mName = name;
                this->mMaxSize = maxSize;
                this->mMsgSize = msgSize;
		this->mMqdt = IPC_ERROR;
		this->mIsCreate = isCreate;
	}

	MessageQueue::~MessageQueue()
	{
		this->close();
		this->unlink();
	}

	bool MessageQueue::open()
	{
		this->close();

		struct mq_attr attr;
		attr.mq_maxmsg = this->mMaxSize;
  		attr.mq_msgsize = this->mMsgSize;
  		attr.mq_flags = 0;
	
		int oflag = O_RDWR;
		
		if(this->mIsCreate)
		{
			oflag |= O_CREAT; 
		}

		this->mMqdt = mq_open (this->mName.c_str(),oflag,0664,&attr);

		return this->mMqdt != -1;	
	}	

	void MessageQueue::close()
	{
		if(this->mMqdt != IPC_ERROR)
		{
			mq_close(this->mMqdt);
		}
	}

	void MessageQueue::unlink()
	{
		if(!this->mName.empty())
		{
			mq_unlink(this->mName.c_str());
		}
	}

	int MessageQueue::send(char *ptr,size_t len)
	{
		return mq_send(this->mMqdt,ptr,len,0);
	}

	size_t MessageQueue::read(char *ptr,size_t len,bool isblocked)
	{
		return mq_receive(this->mMqdt,ptr,len,0);
	}

	void MessageQueue::recvFn(union sigval sv)
	{
		cout<<"RECV FN"<<endl;
	}
}
