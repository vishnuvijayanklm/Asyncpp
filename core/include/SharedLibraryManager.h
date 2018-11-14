#ifndef SHAREDLIBRARYMANAGER_H
#define SHAREDLIBRARYMANAGER_H
#include <Subsystem.h>
#include <defines.h>
#include <SharedLibraryLoader.h>
#include <Stl.h>
namespace Core
{
    class SharedLibraryManager: public ISubsystem
    {
        StlMap<string,shared_ptr<SharedLibraryLoader>> mLoaders;
    public:
        SharedLibraryManager();
        ~SharedLibraryManager();
        void init();
        void shutdown();
        SharedLibraryLoader* loadLibrary(string);
    };
}
#endif // SHAREDLIBRARYMANAGER_H
