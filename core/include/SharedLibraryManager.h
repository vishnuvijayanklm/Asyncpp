#ifndef SHAREDLIBRARYMANAGER_H
#define SHAREDLIBRARYMANAGER_H

#include <util/include/defines.h>
#include <core/include/SharedLibraryLoader.h>
#include <containers/include/Stl.h>

namespace Core
{
    class SharedLibraryManager
    {
        StlMap<string, shared_ptr<SharedLibraryLoader>> mLoaders;

    public:
        SharedLibraryManager();
        ~SharedLibraryManager();
        void init();
        void shutdown();
        SharedLibraryLoader *loadLibrary(string);
    };
}
#endif // SHAREDLIBRARYMANAGER_H
