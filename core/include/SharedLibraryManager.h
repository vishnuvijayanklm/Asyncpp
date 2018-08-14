#ifndef SHAREDLIBRARYMANAGER_H
#define SHAREDLIBRARYMANAGER_H
#include <Subsystem.h>
#include <defines.h>
#include <SharedLibraryLoader.h>
#include <containers/include/Stl.h>
namespace BHI
{
    namespace Core
    {
        class SharedLibraryManager: public ISubsystem
        {
                StlMap<string,shared_ptr<SharedLibraryLoader>> mLoaders;
            public:
                SharedLibraryManager(string);
                ~SharedLibraryManager();
                void init();
                void shutdown();
                SharedLibraryLoader* loadLibrary(string);
        };
    }
}

#endif // SHAREDLIBRARYMANAGER_H
