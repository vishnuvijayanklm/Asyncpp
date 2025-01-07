#ifndef WAITGROUP_H
#define WAITGROUP_H

using namespace std;
namespace Core
{
	class WaitGroup
	{
		mutex mMtx;
		mutex mCondMtx;
		condition_variable mCondVar;
		unsigned int mCounter; 
	public:
		WaitGroup()
		{
			mCounter = 0x00;
		}
		~WaitGroup()
		{
		}

		void add(unsigned int delta = 1)
		{
			lock_guard<mutex> lock(this->mMtx);
			mCounter += delta;
		}

		void done()
		{
			lock_guard<mutex> lock(this->mMtx);
			mCounter--;
			unique_lock<mutex> lockCondition(this->mCondMtx);
			this->mCondVar.notify_all();
		}

		void wait()
		{

			unique_lock<mutex> lock(this->mCondMtx);
			this->mCondVar.wait(lock, [&]{ return (!this->mCounter); });

		}

		unsigned int count()
		{
			return mCounter;
		}
	};
}

#endif
