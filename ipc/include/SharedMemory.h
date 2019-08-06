#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h>   
#include <string>
#include <unistd.h>
#include <iostream>
#include <util/include/defines.h>

using namespace std;

namespace IPC
{
	class SharedMemory
	{
			char* mAddress;
			int mFd;
			size_t mSize;
			string mName;
			bool mIsServer;

			bool map();
			void unmap();
			void close();
		public:
			bool init();
			
			SharedMemory();
			SharedMemory(string name,size_t size,bool = false);
			~SharedMemory();
			
			inline char* begin() const;
			inline char* end() const;
			inline size_t getTotalMemory() const;
	};

	char* SharedMemory::begin() const
	{
		if(this->mAddress != nullptr)
		{
			return this->mAddress; 
		}
		return nullptr;
	}
	char* SharedMemory::end() const
        {
                if(this->mAddress != nullptr)
                {
                        return this->mAddress + this->mSize;
                }
                return nullptr;
        }

	size_t SharedMemory::getTotalMemory() const
	{
		return end() - begin();
	}
}
#endif
