#include "SharedLibraryLoader.h"
namespace Core
{
    SharedLibraryLoader::SharedLibraryLoader(string libraryname)
    {
        LOG_VERBOSE((LOGGER),("SharedLibraryLoader::SharedLibraryLoader>>>"));
        this->mSharedLibraryName = libraryname;
        this->mLibraryHandle = nullptr;
        LOG_VERBOSE((LOGGER),("SharedLibraryLoader::SharedLibraryLoader<<<"));
    }

    SharedLibraryLoader::~SharedLibraryLoader()
    {
        LOG_VERBOSE((LOGGER),("SharedLibraryLoader::~SharedLibraryLoader>>>"));
        this->unloadLibrary();
        LOG_VERBOSE((LOGGER),("SharedLibraryLoader::~SharedLibraryLoader<<<"));
    }

    bool SharedLibraryLoader::loadLibrary(string libraryname)
    {
        if(likely(!libraryname.empty()))
        {
            this->mSharedLibraryName = libraryname;
        }
        this->mLibraryHandle = dlopen(this->mSharedLibraryName.c_str(),RTLD_LAZY);
        return this->mLibraryHandle != nullptr;
    }

    void* SharedLibraryLoader::loadSymbol(string symbolName)
    {
        if(unlikely(this->mLibraryHandle == nullptr))
        {
            return nullptr;
        }

        return dlsym(this->mLibraryHandle,symbolName.c_str());
    }

    void SharedLibraryLoader::unloadLibrary()
    {
        if(likely(this->mLibraryHandle != nullptr))
        {
            dlclose(this->mLibraryHandle);
        }
	this->mLibraryHandle = nullptr;
    }
}
