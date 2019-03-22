#include <atomic>
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
		Node<T> *p = new Node<T>();
		if(p)
		{
			p->mValue = i;
			p->pNext = mHead.load(std::memory_order_relaxed);
			while(!std::atomic_compare_exchange_strong_explicit(&mHead,&p->pNext,p,std::memory_order_release,std::memory_order_relaxed));
			this->mSize++;
		}
	}

	bool pop(T &ret)
	{
		Node<T> *p = nullptr;
		while(mHead.load(std::memory_order_relaxed) && (!std::atomic_compare_exchange_strong_explicit(&mHead,&p,mHead.load()->pNext,std::memory_order_release,std::memory_order_relaxed)));
		if(!p)
		{
			return false;
		}
		ret = p->getValue();
		this->mSize--;
		delete p;
		return true;
	}

	size_t size()
	{
		return this->mSize.load();
	}
};
