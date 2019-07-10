#ifndef IPCMESSAGES_H
#define IPCMESSAGES_H

#include <string.h>

namespace IPC
{
	enum class SHM_USAGE_STATUS
	{
		SHM_FREE,
		SHM_USED
	};

	class ShmHeader
	{
		private:
			char *mBegin;
			char *mEnd;
			char *mCurrent;
			ShmHeader *pNext;
			ShmHeader *pPrev;
			
			SHM_USAGE_STATUS mUsageStatus;
		public:
			ShmHeader()
			{
				this->mBegin = this->mEnd = (char*)this + sizeof(ShmHeader);
				this->init();
				
			}

			ShmHeader(ShmHeader&) = delete;
			~ShmHeader() = default;

			void init()
			{
				this->mUsageStatus = SHM_USAGE_STATUS::SHM_FREE;
                                this->pNext = nullptr;
                                this->pPrev = nullptr;
				this->mCurrent = this->mBegin;
			}

			void reserve(size_t size)
			{
				this->mEnd = this->mBegin + size;
			}
			
			void* operator new(size_t size,void *ptr)
			{
				return ::new (ptr)ShmHeader();
			}

			void operator delete(void *ptr)
			{
				if(ptr != nullptr)
				{
					ShmHeader *pShmHdr = (ShmHeader*)ptr;
					pShmHdr->init();
				}	
			}

			bool isFree() const
			{
				return this->mUsageStatus == SHM_USAGE_STATUS::SHM_FREE;
			}
			
			char* begin() const
			{
				return this->mBegin;
			}

			char* end() const
                        {
                                return this->mEnd;
                        }

			char* current() const
                        {
                                return this->mCurrent;
                        }

			size_t capacity() const
			{
				return this->mEnd - this->mBegin;
			}

			size_t free() const
			{
				return this->mEnd - this->mCurrent;
			}

			size_t size() const
			{
				return this->mCurrent - this->mBegin;
			}

			size_t write(const char *ptr,size_t size)
			{
				this->mUsageStatus = SHM_USAGE_STATUS::SHM_USED;
				size_t capacity = this->capacity();
				if(capacity < size)
				{
					memcpy(this->mBegin,ptr,capacity);
					this->mCurrent += capacity;
					return 1; 
				}
				else
				{
					memcpy(this->mBegin,ptr,size);
					this->mCurrent += size;
					return size;
				}
				return -1;
			}

			size_t read(char *ptr)
			{
				size_t size = this->size();
				if((size > 0) && (ptr != nullptr))	
				{
					memcpy(ptr,begin(),size);
					return size;
				}	
				return -1;
			}
	};
}
#endif
