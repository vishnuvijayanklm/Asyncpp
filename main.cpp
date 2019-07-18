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
#include <async/include/PollManager.h>
#include <async/include/Task.h>

using namespace std;
unique_ptr<Core::Application> pApplication(new Core::Application());
LockFreeQueue<int> Q;
Logger LOGGER;
/*
void threadTest(int i)
{
    while(1)
    {
        char p[3];
        if(msgQ.read(p,4) == -1)
        {
                cout<<"RECV ERROR"<<endl;
        }
        else
        {
                cout<<"RECV "<<p<<endl;
        }
    }
}*/

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
/*
TEST_CASE(TESTCASE0)
{
	CHECK_NE(1,2);
	CHECK_NE(2,2);
	CHECK_EQ(1,2);
	CHECK_EQ(11,11);
}

TEST_CASE(TESTCASE1)
{
	CHECK_NE(1,2);
	CHECK_EQ(2,2);
	CHECK_EQ(11,11);
}*/

int main(int argc,char *argv[])
{
    //Core::NotifyManager *pManager = nullptr;
    try
    {

	LOGGER.setLogFile("Logs","framework.log");
	Async::PollManager::getInstance();
	//std::thread t(threadTest,1);
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
      	while(1)
	{ 
		Async::Task([]()
		{
			cout<<"Hai 1"<<endl;
			char *ptr = new char[10];
			return ptr;
		},
		[](char *ptr)
		{
			printf("Got %p\n",ptr);	
			delete ptr;
			ptr = nullptr;
		})
		.then([]()
		{
			cout<<"Hai 2"<<endl;
			return 2;
		},
		[](int x)
		{
			cout<<"Got "<<x<<endl;
		})
		.then([]()
		{	
			cout<<"Hai 3"<<endl;
			return 'A';
		},
		[](char c)
		{
			cout<<"Got "<<c<<endl;
		})
		.then([]()
                {
                        cout<<"Hai 4"<<endl;
                	return "Hai 4";
		},
		[](string s)
                {
                        cout<<"Got "<<s<<endl;
                })
		.then([]()
		{
			cout<<"Hai 5"<<endl;
		}).execute();
		usleep(100000);
	}
	 //Async::Task([](){cout<<"Hai1"<<endl;
			//		return 1;
	//			});
				/*.then([]()
				{
					cout<<"Hai2"<<endl;
				})
				.then([]()
				{
					cout<<"Hai3"<<endl;
				})
				.then([]()
				{
					cout<<"Hai4"<<endl;
				})
				.then([]()
				{
					cout<<"Hai5"<<endl;
				})
				.execute();*/
	//task.execute();

	return 0;
	if(unlikely(pApplication.get() == nullptr))
        {
            LOG_ERRORNP((LOGGER),("Failed to get appliaction object"));
            return -1;
        }
        pApplication->init();
        pApplication->onApplicationStart(argc,argv);
        //pApplication->registerSignal(SIGINT,signalCatch);
	IPC::MessageQueue msgQ("/mq",100,sizeof(IPC::ShmHeader*),true);
	msgQ.open();
	msgQ.read([](shared_ptr<char> ptr, size_t len)
	{
		long p;
		memcpy(&p,ptr.get(),len);
		IPC::ShmHeader *pShm = (IPC::ShmHeader*)p;

		char pp[100];
		pShm->read(pp);	
		pp[pShm->size()] = 0x00;
		cout<<"Data "<<pp<<" Free "<<pShm->free()<<endl;
	});
	
	//IPC::Transceiver("/1",1024,10,true);	
	int i = 0;
	while(1)
	{
		char *Shmptr = new char[sizeof(IPC::ShmHeader) +10];
		IPC::ShmHeader *pShm = new (Shmptr) IPC::ShmHeader();
		pShm->reserve(10);
		
		string s = "Hai_" + to_string(i++);	
		pShm->write(s.c_str(),s.length());
		long x = (long)pShm;	
		msgQ.send((char*)&pShm,sizeof(IPC::ShmHeader*));
	
		//sleep(1);
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
		//usleep(1);
		usleep(100000);
	}

	return 0;
	
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
