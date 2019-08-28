#include <atomic>
#include <util/include/defines.h>

extern Logger LOGGER;
template<typename T>
class LockFreeQueue
{
	struct Node
	{
		public:
			const T mValue;
		public:
			Node *pNext;
			Node(const T& value) : mValue(value)
			{
				this->pNext = nullptr;
			}
	
			~Node()
			{
				this->pNext = nullptr;
			}

			T getValue()
			{
				return this->mValue;
			}
	};

	mutex m_conditionMutex;
        condition_variable m_conditionVariable;

	std::atomic<Node*> mHead;
	std::atomic<Node*> mTail;
	std::atomic<size_t> mSize;
	public:
	LockFreeQueue()
	{
		this->mSize = 0;
	}
	
	void push(T value)
	{
		Node *pNode = new Node(value);
		if(likely(pNode != nullptr))
		{
			if(this->mHead.load() == nullptr)
			{
				this->mHead.store(pNode);
				this->mTail.store(pNode);
			}
			else
			{
				Node *pTemp = nullptr;
				while(!(pTemp = atomic_exchange(&this->mTail,pNode)))
				{
					this_thread::yield();
				}	
				if(pTemp != nullptr)
				{
					pTemp->pNext = pNode;
				}
				/*
				//pNode->pNext = mHead.load(std::memory_order_relaxed);
				while(!std::atomic_compare_exchange_strong_explicit(&mHead,&pNode->pNext,pNode,std::memory_order_release,std::memory_order_relaxed))
				{
					this_thread::yield();
				}*/
			}
			this->mSize++;
			this->m_conditionVariable.notify_one();
			//LOG_INFO((LOGGER),("Inserted %p %p",pNode,value.get()));
		}
	}

	bool pop(T &value,bool isblocked = false)
	{
		if(this->empty())
		{
			if(isblocked)
			{
				unique_lock<mutex> lock(this->m_conditionMutex);
                                this->m_conditionVariable.wait(lock,[&]{return (!this->empty());});
			}
			else
			{
				return false;
			}
		}

		Node *pNode = nullptr;
		while(mHead.load(std::memory_order_relaxed) && (!std::atomic_compare_exchange_strong_explicit(&mHead,&pNode,mHead.load()->pNext,std::memory_order_release,std::memory_order_relaxed)))
		{
			this_thread::yield();
		}

		if(unlikely(pNode == nullptr))
		{
			return false;
		}

		value = pNode->getValue();
		//LOG_INFO((LOGGER),("Poped %p %p",pNode,value.get()));
		this->mSize--;
		delete pNode;
		pNode = nullptr;
		return true;
	}

	size_t size()
	{
		return this->mSize.load();
	}

	bool empty()
	{
		return this->size() == 0;
	}
};

