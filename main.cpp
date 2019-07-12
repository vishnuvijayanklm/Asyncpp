#include <iostream>
#include <core/include/Application.h>
#include <core/include/Exception.h>
#include <memory>
#include <unistd.h>
#include <util/include/Runnable.h>
#include <util/include/File.h>
#include <util/include/defines.h>
#include <core/include/SharedLibraryLoader.h>
#include <core/include/SharedLibraryManager.h>
#include <containers/include/Stl.h>
#include <core/include/ThreadPool.h>
#include <containers/include/LockFree.h>
#include <ipc/include/SharedMemory.h>
#include <ipc/include/Transceiver.h>
#include <ipc/include/IPCMessage.h>
#include <ipc/include/MessageQueue.h>

LockFreeQueue<int> Q;
Logger LOGGER;
IPC::MessageQueue msgQ(0x00100);
void threadTest(int i)
{
    while(1)
    {
	char p[3];
	cout<<"RECV ==== "<<msgQ.read(p,3,true)<<endl;
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
	std::thread t(threadTest,1);
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
	msgQ.open();
	while(1)
	{
		
		cout<<"SIZE ======= "<<sizeof(IPC::ShmHeader)<<endl;
		//IPC::Transceiver transceiver("/1",1024,5,true);
		cout<<"=======ID==========="<<std::hex<<msgQ.getMsqId()<<endl;
		char ptr[] = "Hai";
		cout<<"=======SEND========="<<msgQ.send(ptr,3)<<endl;
		/*
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
		cout<<endl;*/
		usleep(100000);
		//sleep(1);
	}

	return 0;
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
