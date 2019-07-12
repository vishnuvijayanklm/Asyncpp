#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <core/include/Exception.h>
#include <util/include/defines.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

using namespace std;

namespace IPC
{
	class MessageQueue
	{
			key_t mKey;
			int mMsqId;

		public:
			MessageQueue(key_t);
			~MessageQueue();
			
			void open();
			void close();

			inline int getMsqId() const;
			
			size_t send(char *ptr,size_t len);
			size_t read(char *ptr,size_t len,bool isblocked = false);
	};

	int MessageQueue::getMsqId() const
	{
		return this->mMsqId;
	}
}
#endif
