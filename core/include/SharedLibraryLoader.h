#ifndef SHAREDLIBRARYLOADER_H
#define SHAREDLIBRARYLOADER_H
#include <string>
#include <dlfcn.h>
#include <iostream>
#include <defines.h>
using namespace std;

namespace Core
{
    class SharedLibraryLoader
    {
        string mSharedLibraryName;

        void *mLibraryHandle;
    public:
        SharedLibraryLoader(string = "");
        ~SharedLibraryLoader();
        bool loadLibrary(string = "");
        void* loadSymbol(string);
        void unloadLibrary();
    };
}

#endif // SHAREDLIBRARYLOADER_H
