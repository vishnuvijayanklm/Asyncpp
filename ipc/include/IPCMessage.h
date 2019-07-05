#ifndef IPCMESSAGES_H
#define IPCMESSAGES_H

class MessageHeader
{
	private:
		unsigned short mSrcId;
		unsigned short mDestId;
		unsigned short mStatusId;
		unsigned short mStatusCode;
		
		unsigned long mTransactionId;
		
		
	public:
		
};
#endif
