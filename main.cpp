#include <iostream>
#include <Application.h>
#include <Exception.h>
#include <memory>
#include <unistd.h>
#include <Runnable.h>
#include <File.h>
#include <defines.h>
#include <SharedLibraryLoader.h>
#include <SharedLibraryManager.h>
#include <Stl.h>
Logger LOGGER;
void threadTest(int i)
{
    while(1)
    {
        cerr<<"In threadTest "<<i<<endl;
        sleep(1);
    }
}
using namespace std;
unique_ptr<Core::Application> pApplication(new Core::Application(("Application")));
void signalCatch(int s)
{
    cerr<<"Signal caught "<<s<<endl;
    cout<<"Calling"<<endl;
    //LOG_INFONP((LOGGER),("Exception caught %d",s));
   /*  if(pApplication.get())
     	pApplication->shutdown();*/
    cout<<"Exiting......."<<endl;
    exit(0);
}
void fileTest()
{
    Util::File f;
    f.open("1.txt");
    if(f.is_open())
    {
        cout<<"File Open"<<endl;
    }
    else
    {
        cout<<"Failed "<<endl;
    }
}
int main(int argc,char *argv[])
{
    //Core::NotifyManager *pManager = nullptr;
    try
    {
        LOGGER.setLogFile("Logs","framework.log");

        /*
        StlTimedMap<int,int> timedMap;
        int i =0;
        while(1)
        {
            //LOG_INFONP((LOGGER),("Inserting %d",i));
            timedMap.insert(i,i,1);
            i++;
            //usleep(100);
        }
        while(1)
        {
            try
            {
                pApplication->registerSubsystem(new Core::SharedLibraryManager("SharedLibraryManager"));
            }
            catch(exception &e)
            {
                LOG_ERRORNP((LOGGER),("Exception caught %s",e.what()));
            }

            Core::SharedLibraryManager* pSharedLibraryManager = (Core::SharedLibraryManager*)pApplication->getSubsystem("SharedLibraryManager");

            if(pSharedLibraryManager)
            {
                Core::SharedLibraryLoader *pLibraryLoader = pSharedLibraryManager->loadLibrary("./shared.so");
                //cout<<"SharedLibraryLoader  "<<pLibraryLoader<<endl;
                auto x = (void(*)())pLibraryLoader->loadSymbol("create_object");
                x();
            }
        }
        return 0;*/
        if(!pApplication.get())
        {
            LOG_ERRORNP((LOGGER),("Failed to get appliaction object"));
            return -1;
        }
        pApplication->init();
        pApplication->onApplicationStart(argc,argv);
        pApplication->registerSignal(SIGINT,signalCatch);
        //fileTest();
        //return 0;
        pApplication->run();
        pApplication->shutdown();
    }
    catch(Core::Exception &e)
    {
        LOG_CRITICALNP((LOGGER),("Exception : %d : %s %s",e.getCode(),e.what(),e.getAdditionalInfo()));
    }
    catch(exception &e)
    {
        LOG_CRITICALNP((LOGGER),("Exception : %s",e.what()));
    }
    return 0;
}
