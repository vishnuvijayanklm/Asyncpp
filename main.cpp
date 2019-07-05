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
#include <ThreadPool.h>
#include <LockFree.h>
#include <SharedMemory.h>
LockFreeQueue<int> Q;
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
unique_ptr<Core::Application> pApplication(new Core::Application());
void signalCatch(int s)
{
    cerr<<"Signal caught "<<s<<endl;
    cout<<"Calling"<<endl;
    LOG_INFONP((LOGGER),("Exception caught %d",s));
   /*  if(pApplication.get())
     	pApplication->shutdown();*/
    //cout<<"Exiting......."<<endl;
    //pApplication->registerSignal(s,nullptr);
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
	StlMap<int,int,std::recursive_mutex> myMap;	
	int val = 0;
	//StlList<int> list;
	while(val < 100)
        {
		cout<<"Inserting "<<val<<endl;
		myMap.insert(val,val);
		cout<<"Inserted "<<val<<endl;
        	//list.insert(val++);
        	//list.push_front(val++);
        	val++;
	}
	StlMap<int,int,std::recursive_mutex> temp;	
	temp = myMap;
	LOG_INFONP((LOGGER),("myMap Size %d temp Size %d",myMap.size(),temp.size()));
	return 0;
	myMap.startGet();
	myMap.erase(99);
	myMap.insert(101,101);
	myMap.stopGet();
	myMap.startGet();
	while(myMap.getNextElement(val))
	{
		LOG_INFONP((LOGGER),("Value from list %d",val));
	}
	myMap.stopGet();
	return 0;*/
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
        if(unlikely(pApplication.get() == nullptr))
        {
            LOG_ERRORNP((LOGGER),("Failed to get appliaction object"));
            return -1;
        }

	while(1)
	{
		IPC::SharedMemory shm("/1",1024,true);
	
		int i = 0;	
		unsigned char *ptr = shm.begin();
		
		while(ptr && (ptr != shm.end()))
		{
			*ptr++ = 'A' + (i++ % 26);
		}
		ptr = shm.begin();
		while(ptr && (ptr != shm.end()))
                {
			cout<<*ptr;
                        ptr++;
                }
		cout<<endl;
		sleep(10);
	}

        pApplication->init();
        pApplication->onApplicationStart(argc,argv);
        pApplication->registerSignal(SIGINT,signalCatch);
	Core::SharedLibraryManager* pSharedLibraryManager = (Core::SharedLibraryManager*)pApplication->getSubsystem(SHAREDLIBRARY_MANAGER);
	if(pSharedLibraryManager)
	{
		Core::SharedLibraryLoader *pLibraryLoader = pSharedLibraryManager->loadLibrary("./shared.so");
		if(pLibraryLoader)
		{	
			auto x = (void(*)())pLibraryLoader->loadSymbol("create_object");
			if(x)
			{
				x();
			}
		}
	}
        pApplication->run();
    }
    catch(Core::Exception &e)
    {
        LOG_CRITICALNP((LOGGER),("Exception : %d : %s %s Errno %d",e.getCode(),e.what(),e.getAdditionalInfo(),e.getErrno()));
    }
    catch(exception &e)
    {
        LOG_CRITICALNP((LOGGER),("Exception : %s",e.what()));
    }
    return 0;
}
