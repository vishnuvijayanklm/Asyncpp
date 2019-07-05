#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h>   
#include <string>
#include <Exception.h>
#include <unistd.h>
#include <iostream>
using namespace std;

namespace IPC
{
	class SharedMemory
	{
			unsigned char* mAddress;
			int mFd;
			size_t mSize;
			string mName;

			void init(bool);
			void map();
			void unmap();
			void close();
		public:
			SharedMemory();
			SharedMemory(string name,size_t size,bool = false);
			~SharedMemory();
			
			inline unsigned char* begin() const;
			inline unsigned char* end() const;
	};

	unsigned char* SharedMemory::begin() const
	{
		if(this->mAddress != nullptr)
		{
			return this->mAddress; 
		}
		return nullptr;
	}
	unsigned char* SharedMemory::end() const
        {
                if(this->mAddress != nullptr)
                {
                        return this->mAddress + this->mSize;
                }
                return nullptr;
        }
}
#endif
