#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <core/include/Exception.h>
#include <util/include/defines.h>
#include <mqueue.h>

using namespace std;

namespace IPC
{
	class MessageQueue
	{
			mqd_t mMqdt;
			bool mIsCreate;
			size_t mMaxSize;
			size_t mMsgSize;
			string mName;

			void unlink();

			void recvFn(union sigval sv);
		public:
			MessageQueue(string name,size_t maxSize,size_t msgSize,bool isCreate = false);
			~MessageQueue();
		
			bool open();	
			void close();
		
			inline mqd_t id() const;	
			
			int send(char *ptr,size_t len);
			size_t read(char *ptr,size_t len,bool isblocked = false);
	};

	mqd_t MessageQueue::id() const
	{
		return this->mMqdt;
	}
}
#endif
