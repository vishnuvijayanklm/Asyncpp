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
		this->init(isServer);
	}

	SharedMemory::~SharedMemory()
	{
		this->unmap();
		this->close();
	}

	void SharedMemory::init(bool isServer) throw(INVALID_SHARED_MEMORY_SIZE_CONFIGURED,SHARED_MEMORY_CREATE_FAILED,SHARED_MEMORY_RESIZE_FAILED)
	{

		if(this->mSize <= 0)
		{
			throw INVALID_SHARED_MEMORY_SIZE_CONFIGURED();
		}

		int flags = isServer ? O_CREAT : 0;
		flags |= O_RDWR;

		this->mFd = shm_open(this->mName.c_str(),flags,S_IRUSR | S_IWUSR);

		if(this->mFd == -1)
		{
			throw SHARED_MEMORY_CREATE_FAILED();
		}

		if(isServer && (ftruncate(this->mFd,this->mSize) == -1))
		{
			::close(this->mFd);
			this->mFd = -1;
			shm_unlink(this->mName.c_str());
			throw SHARED_MEMORY_RESIZE_FAILED();
		}
		
		this->map();
	}

	void SharedMemory::map() throw(SHARED_MEMORY_MAPPING_FAILED)
	{
		int access = PROT_READ | PROT_WRITE;
		
		void* addr = ::mmap(NULL,this->mSize,access,MAP_SHARED,this->mFd, 0);
		if (addr == MAP_FAILED)
		{
			throw SHARED_MEMORY_MAPPING_FAILED();
		}

		this->mAddress = static_cast<char*>(addr);
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
