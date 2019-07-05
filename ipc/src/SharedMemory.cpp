#include <SharedMemory.h>
#include <iostream>
#include <errno.h>
namespace IPC
{
	SharedMemory::SharedMemory(string name,size_t size,bool isServer)
	{
		this->mFd = -1;
		this->mSize = size;
		this->mAddress = nullptr;
		this->mName = name;
		this->init(isServer);
	}

	SharedMemory::~SharedMemory()
	{
		this->unmap();
		this->close();
	}

	void SharedMemory::init(bool isServer)
	{
		int flags = isServer ? O_CREAT : 0;
		flags |= O_RDWR;
	
		this->mFd = shm_open(this->mName.c_str(),flags,S_IRUSR | S_IWUSR);

		if(this->mFd == -1)
		{
			throw FAILED_CREATE_SHARED_MEMORY();
		}

		if(isServer && (ftruncate(this->mFd,this->mSize) == -1))
		{
			::close(this->mFd);
			this->mFd = -1;
			shm_unlink(this->mName.c_str());
			throw RESIZE_SHARED_MEMORY_FAILED();
		}
		
		this->map();
	}

	void SharedMemory::map()
	{
		int access = PROT_READ | PROT_WRITE;
		
		void* addr = ::mmap(NULL,this->mSize,access,MAP_SHARED,this->mFd, 0);
		if (addr == MAP_FAILED)
		{
			throw SHARED_MEMORY_MAP_FAILED();
		}

		this->mAddress = static_cast<unsigned char*>(addr);
	}

	void SharedMemory::unmap()
	{
		if(this->mAddress)
		{
                	munmap(this->mAddress,this->mSize);
			this->mAddress = nullptr;
		}
	}

	void SharedMemory::close()
	{
		if(this->mFd != -1)
		{
			::close(this->mFd);
               		this->mFd = -1;
                	shm_unlink(this->mName.c_str());
		}
	}
}
