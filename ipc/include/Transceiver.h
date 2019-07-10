#ifndef IPCTRANSCEIVER_H
#define IPCTRANSCEIVER_H

#include <string>
#include <ipc/include/SharedMemory.h>
#include <ipc/include/IPCMessage.h>
#include <containers/include/Stl.h>

using namespace std;
namespace IPC
{
	class Transceiver
	{
			size_t mSegmentCount;
			size_t mSegmentSize;
			StlQueue<ShmHeader*> mShmQueue;
						
			SharedMemory mSharedMemory;

			void init();
		public:
			Transceiver(string name,size_t segmentSize,size_t segmentCount,bool isServer);
			~Transceiver();
	};
}
#endif
