#include <ipc/include/Transceiver.h>
#include <array>

namespace IPC
{
	Transceiver::Transceiver(string name,size_t segmentSize,size_t segmentCount,bool isServer):mSharedMemory(name,(segmentCount * segmentSize) + (segmentCount * sizeof(ShmHeader)),isServer)
	{
		this->mSegmentSize = segmentSize;
		this->mSegmentCount = segmentCount;
		this->init();
	}

	Transceiver::~Transceiver()
	{
		this->mShmQueue.clear();
	}

	void Transceiver::init()
	{
		size_t totalSize = (this->mSegmentSize * this->mSegmentCount) + (this->mSegmentCount *  sizeof(ShmHeader));

		if(totalSize != this->mSharedMemory.getTotalMemory())
		{
			return;
		}

		char p[1024];
		for(int i = 0; i< 1024;i++)
		{
			p[i] = 'A' + (i % 26);
		}


		char *Shmptr = this->mSharedMemory.begin();
		for(int i = 0; i< this->mSegmentCount ; i++)
		{
			ShmHeader *pShm = new (Shmptr) ShmHeader();
			pShm->reserve(this->mSegmentSize);	
			Shmptr += (sizeof(ShmHeader) + this->mSegmentSize);

			pShm->write(p,1024);
			this->mShmQueue.push(pShm);

			printf("PTR %p BEGIN %p END %p CAPACITY  %lu USED %lu FREE %lu ISFREE %d\n",pShm,pShm->begin(),pShm->end(),pShm->capacity(),pShm->size(),pShm->free(),pShm->isFree());
		}

		ShmHeader *pShm = nullptr;

		while(this->mShmQueue.pop(pShm))
		{
			char pp[pShm->size() +1];
			pShm->read(pp);

			delete pShm;
			printf("PTR %p BEGIN %p END %p CAPACITY  %lu USED %lu FREE %lu ISFREE %d\n",pShm,pShm->begin(),pShm->end(),pShm->capacity(),pShm->size(),pShm->free(),pShm->isFree());
			pShm = nullptr;
			delete pShm;
		}		
	}
}
