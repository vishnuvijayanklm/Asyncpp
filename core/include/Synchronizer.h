#ifndef SYNCHRONIZER_H
#define SYNCHRONIZER_H
namespace Core
{
	class SyncKey
	{
		unsigned int mKey;
		public:
		SyncKey():mKey(1)
		{
		}
		SyncKey(unsigned int key) : mKey(key)
		{
			
		}

		SyncKey(const SyncKey&) = default;
		SyncKey& operator=(const SyncKey&) = default;

		unsigned int getKey() const
		{
			return this->mKey;
		}
	};
	class Synchronizer
	{
		static unsigned int SYNC_DISABLED;
		static unsigned int mSyncKey;

		public:
		
		static SyncKey getSyncKey() 
		{
			return SyncKey(mSyncKey++);
		}
	};
}

#endif
