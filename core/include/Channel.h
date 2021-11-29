#ifndef CHANNEL_H
#define CHANNEL_H
#include <containers/include/Stl.h>

using namespace std;
namespace Core
{
	enum class CHANNEL_STATUS
	{
		OPEN,
		CLOSED
	};

	template <typename T>
	class Channel
	{
		StlQueue<T> mChannelData;
		CHANNEL_STATUS mChannelStatus;

	public:
		Channel()
		{
			this->mChannelStatus = CHANNEL_STATUS::OPEN;
		}
		~Channel()
		{
		}

		void operator<<(T val)
		{
			if (this->mChannelStatus == CHANNEL_STATUS::OPEN)
			{
				this->mChannelData.push(val);
			}
		}

		void close()
		{
			this->mChannelStatus = CHANNEL_STATUS::CLOSED;
		}

		size_t size()
		{
			return this->mChannelData.size();
		}
		template <typename T1>
		friend void operator<<(T1 &x, Channel<T1> &);
	};

	template <typename T>
	void operator<<(T &x, Channel<T> &c)
	{
		if (c.mChannelStatus == CHANNEL_STATUS::OPEN)
		{
			if(!c.mChannelData.pop(x, true)) 
			{
				// throw exception here
			}
		}
	}

}

#endif
