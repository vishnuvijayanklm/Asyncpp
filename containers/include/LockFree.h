#include <atomic>
#include <util/include/defines.h>
template<typename T>
class LockFreeQueue
{
	template<typename T1>
		struct Node
		{
			T1 mValue;
			Node *pNext;

			Node()
			{
				this->pNext = nullptr;
			}

			~Node()
			{
				this->pNext = nullptr;
			}

			T1 getValue()
			{
				return this->mValue;
			}
		};

	std::atomic<Node<T>*> mHead;
	std::atomic<size_t> mSize;
	public:
	LockFreeQueue()
	{
		this->mSize = 0;
	}
	void push(T i)
	{
		Node<T> *pNode = new Node<T>();
		if(likely(pNode))
		{
			pNode->mValue = i;
			pNode->pNext = mHead.load(std::memory_order_relaxed);
			while(!std::atomic_compare_exchange_strong_explicit(&mHead,&pNode->pNext,pNode,std::memory_order_release,std::memory_order_relaxed))
			{
				this_thread::yield();
			}
			this->mSize++;
		}
	}

	bool pop(T &ret)
	{
		Node<T> *pNode = nullptr;
		while(mHead.load(std::memory_order_relaxed) && (!std::atomic_compare_exchange_strong_explicit(&mHead,&pNode,mHead.load()->pNext,std::memory_order_release,std::memory_order_relaxed)))
		{
			this_thread::yield();
		}
		if(unlikely(!pNode))
		{
			return false;
		}
		ret = pNode->getValue();
		this->mSize--;
		delete pNode;
		pNode = nullptr;
		return true;
	}

	size_t size()
	{
		return this->mSize.load();
	}
};

