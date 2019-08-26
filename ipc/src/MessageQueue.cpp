#include <ipc/include/MessageQueue.h>
#include <async/include/PollManager.h>
#include <core/include/Application.h>
#include <mqueue.h>
extern unique_ptr<Core::Application> pApplication;
namespace IPC
{
	MessageQueue::MessageQueue(string name,size_t maxSize,size_t msgSize,bool isCreate)
	{
		this->mName = name;
                this->mMaxSize = maxSize;
                this->mMsgSize = msgSize;
		this->mMqdt = IPC_ERROR;
		this->mIsCreate = isCreate;
		this->mCallBackFn = nullptr;
	
	}

	MessageQueue::~MessageQueue()
	{
		this->close();
		this->unlink();
	}

	void MessageQueue::initialize()
	{
		this->close();
		this->unlink();
	}

	bool MessageQueue::open()
	{
		this->initialize();
		struct mq_attr attr;
		attr.mq_maxmsg = this->mMaxSize;
  		attr.mq_msgsize = this->mMsgSize;
  		attr.mq_flags = 0;
	
		int oflag = O_RDWR | O_NONBLOCK;
		if(this->mIsCreate)
		{
			this->unlink();
			oflag |= O_CREAT;
		}

		this->mMqdt = mq_open (this->mName.c_str(),oflag,0664,&attr);

		if(this->mMqdt == -1)
		{	
			LOG_ERROR((LOGGER),("Failed to open Message Queue [%d]",errno));
			return false;
		}
		return true;
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

	void MessageQueue::recv(CallBack callBackFn)
	{
		if(this->mMqdt != -1)
		{
			Async::PollManager::getInstance()->addEventListener(this->mMqdt,this);
		}
		this->mCallBackFn =  callBackFn;	
	}	
	
	int MessageQueue::send(char *ptr,size_t len)
	{
		return mq_send(this->mMqdt,ptr,len,0);
	}

		
	size_t MessageQueue::read(char *ptr,size_t len)
	{
		return mq_receive(this->mMqdt,ptr,len,0);
	}

	
	void MessageQueue::onEventReceived()
	{
		while(this->getAttributes().mq_curmsgs)
		{
			std::shared_ptr<char> ptr(new char[this->mMsgSize],std::default_delete<char[]>());
			if(ptr != nullptr)
			{
				size_t len = this->read(ptr.get(),this->mMsgSize);
				if(len != -1)
				{
					Async::Task().add(bind(&MessageQueue::onRecv,this,ptr,len)).execute_sync(this->getSyncKey());
				}	
			}
		}
	}
}
