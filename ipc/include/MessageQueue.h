#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <core/include/Exception.h>
#include <util/include/defines.h>
#include <async/include/EventListener.h>
#include <core/include/NotifyManager.h>
#include <mqueue.h>
#include <functional>

using namespace std;

namespace IPC
{
	class MessageQueue : Async::IEventListener
	{
			mqd_t mMqdt;

			bool mIsCreate;
			size_t mMaxSize;
			size_t mMsgSize;
			
			typedef std::function<void(shared_ptr<char>,size_t)> CallBack;
			CallBack mCallBackFn;
			
			string mName;
			Core::NotifyManager *mpNotifyManager;		

	
			void unlink();
				
		public:
			MessageQueue(string name,size_t maxSize,size_t msgSize,bool isCreate = false);
			~MessageQueue();

			bool open();	
			void close();
			void read(CallBack);
			
			inline mqd_t id() const;	
			inline mq_attr getAttributes();
			inline void callBack(shared_ptr<char>&,size_t);

			void onEventReceived() override;
			
			int send(char *ptr,size_t len);
			size_t read(char *ptr,size_t len);
	};

	mqd_t MessageQueue::id() const
	{
		return this->mMqdt;
	}

	mq_attr MessageQueue::getAttributes()
	{
		mq_attr attr;
		mq_getattr(this->mMqdt,&attr);
		return attr;
	}
	
	void MessageQueue::callBack(shared_ptr<char> &ptr,size_t len)
	{
		if(this->mCallBackFn != nullptr)
		{
			this->mCallBackFn(ptr,len);
		}
	}
}
#endif
