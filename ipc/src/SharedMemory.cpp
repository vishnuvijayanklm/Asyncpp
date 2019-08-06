#include <ipc/include/SharedMemory.h>
#include <iostream>
#include <errno.h>
namespace IPC
{
	SharedMemory::SharedMemory()
	{
		
	}

	SharedMemory::SharedMemory(string name,size_t size,bool isServer)
	{
		this->mFd = -1;
		this->mSize = size;
		this->mAddress = nullptr;
		this->mName = name;
		this->mIsServer = isServer;
	}

	SharedMemory::~SharedMemory()
	{
		this->unmap();
		this->close();
	}

	bool SharedMemory::init()
	{

		if(this->mSize <= 0)
		{
			LOG_ERROR((LOGGER),("Invalid SharedMemory size [%d]",this->mSize));
			return false;
		}

		int flags = this->mIsServer ? O_CREAT : 0;
		flags |= O_RDWR;

		this->mFd = shm_open(this->mName.c_str(),flags,S_IRUSR | S_IWUSR);

		if(this->mFd == -1)
		{
			LOG_ERROR((LOGGER),("Failed to open shared Memory [%d]",errno));
			return false;
		}

		if(this->mIsServer && (ftruncate(this->mFd,this->mSize) == -1))
		{
			::close(this->mFd);
			this->mFd = -1;
			shm_unlink(this->mName.c_str());
			LOG_ERROR((LOGGER),("SharedMemory resize failed [%d]",errno));
			return false;
		}
		
		return this->map();
	
	}

	bool SharedMemory::map()
	{
		int access = PROT_READ | PROT_WRITE;
		
		void* addr = ::mmap(NULL,this->mSize,access,MAP_SHARED,this->mFd, 0);
		
		if (addr == MAP_FAILED)
		{
			LOG_ERROR((LOGGER),("SharedMemory mapping failed [%d]",errno));
		}

		this->mAddress = static_cast<char*>(addr);
		
		return this->mAddress != nullptr;
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
