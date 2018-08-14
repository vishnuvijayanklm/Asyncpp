#ifndef STL_H
#define STL_H

#include <queue>
#include <map>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <memory>
using namespace std;
using namespace chrono;
template<class T>
class StlQueue:private queue<T>
{
    int m_size;
public:
    StlQueue()
    {
        this->m_size = 0;
    }

    ~StlQueue()
    {

    }

    void push(T& value)
    {
        lock_guard<mutex> lock(this->m_mtx);
        queue<T>::push(value);
        this->m_conditionVariable.notify_one();
    }

    void push(T&& value)
    {
        lock_guard<mutex> lock(this->m_mtx);
        queue<T>::push(move(value));
        this->m_conditionVariable.notify_one();
    }
    bool pop(T &value,bool isblocked = false)
    {
        if(this->empty())
        {
            if(isblocked)
            {
                unique_lock<mutex> lock(this->m_conditionMutex);
                this->m_conditionVariable.wait(lock,[&] {return (!this->empty());});
            }
            else
            {
                return false;
            }
        }
        lock_guard<mutex> lock(this->m_mtx);
        if(queue<T>::size())
        {
            value = move(queue<T>::front());
            queue<T>::pop();
            return true;
        }
        return false;
    }
    size_t size()
    {
        lock_guard<std::mutex> lock(this->m_mtx);
        return queue<T>::size();
    }

    typedef bool(*unaryPredicate)(T);

    bool roundrobinfind(T &value,function<bool(T)> unaryPredicate)
    {
        lock_guard<mutex> lock(this->m_mtx);
        size_t qsize = queue<T>::size();
        while(qsize--)
        {
            value = queue<T>::front();
            queue<T>::pop();
            queue<T>::push(value);
            if(unaryPredicate(value))
            {
                return true;
            }
        }
        return false;
    }

    bool remove(T value)
    {
        lock_guard<mutex> lock(this->m_mtx);
        T temp;
        size_t qsize = queue<T>::size();
        while(qsize--)
        {
            temp = queue<T>::front();
            queue<T>::pop();
            if(temp==value)
            {
                return true;
            }
            queue<T>::push(value);
        }
        return false;
    }

    void clear()
    {
        lock_guard<mutex> lock(this->m_mtx);
        size_t qsize = queue<T>::size();
        while(qsize--)
        {
            queue<T>::pop();
        }
    }

private:

    mutex m_mtx;
    mutex m_conditionMutex;
    condition_variable m_conditionVariable;
};

template<typename T>
class Getter
{

    T& m_container;
public:
    Getter(T& container):m_container(container)
    {
        m_container.startGet();
    }
    ~Getter()
    {
        m_container.stopGet();
    }
};
template<typename Key,typename Value>
class StlMap:private map<Key,Value>
{
    public:
           StlMap()
           {
           }
           ~StlMap()
           {
           }
           void insert(Key mapKey,Value val)
           {
                lock_guard<mutex> lock(this->m_mtx);
                map<Key,Value>::insert(pair<Key,Value>(mapKey,val));
           }

           bool erase(Key key,Value &val)
           {
                lock_guard<mutex> lock(this->m_mtx);
                it = map<Key,Value>::find(key);
                if(it != map<Key,Value>::end())
                {
                    val = it->second;
                    map<Key,Value>::erase(it);
                    return true;
                }
                return false;
           }
           bool erase(Key key)
           {
               lock_guard<mutex> lock(this->m_mtx);
               it = map<Key,Value>::find(key);
               if(it != map<Key,Value>::end())
               {
                   map<Key,Value>::erase(it);
                   return true;
               }
               return false;
           }

           bool find(Key key,Value &val)
           {
                lock_guard<mutex> lock(this->m_mtx);
                it = map<Key,Value>::find(key);
                if(it != map<Key,Value>::end())
                {
                    val = it->second;
                    return true;
                }
                return false;
           }

           bool find(Key key)
           {
                lock_guard<mutex> lock(this->m_mtx);
                it = map<Key,Value>::find(key);
                if(it != map<Key,Value>::end())
                {
                    return true;
                }
                return false;
           }
           void clear()
           {
               lock_guard<mutex> lock(this->m_mtx);
               map<Key,Value>::clear();
           }

           void startGet()
           {
               this->m_mtx.lock();
               it = map<Key,Value>::begin();
           }

           bool getNextElement(Value &val)
           {
                if(it != map<Key,Value>::end())
                {
                    val = it->second;
                    ++it;
                    return true;
                }
                else
                {
                    return false;
                }
           }
           bool getNextElement(Key &key,Value &val)
           {
                if(it != map<Key,Value>::end())
                {
                    key = it->first;
                    val = it->second;
                    ++it;
                    return true;
                }
                else
                {
                    return false;
                }
           }
           void stopGet()
           {
               this->m_mtx.unlock();
           }
           bool removeFirstElement(Value &val)
           {
               lock_guard<mutex> lock(this->m_mtx);
               it = map<Key,Value>::begin();
               if(it != map<Key,Value>::end())
               {
                   val = move(it->second);
                   map<Key,Value>::erase(it);
                   return true;
               }
               return false;
           }

           bool removeFirstElement(Key &key,Value &val)
           {
               lock_guard<mutex> lock(this->m_mtx);
               it = map<Key,Value>::begin();
               if(it != map<Key,Value>::end())
               {
                   key = it->first;
                   val = it->second;
                   map<Key,Value>::erase(it);
                   return true;
               }
               return false;
           }
           size_t size()
           {
               lock_guard<mutex> lock(this->m_mtx);
               return map<Key,Value>::size();
           }

           private:
            //iterator it;
            typename map<Key,Value>::iterator it;
            std::mutex m_mtx;
};


template<typename Key,typename Value>
class StlTimedMap:private map<Key,Value>
{
    public:
           StlTimedMap()
           {
               this->m_timeExpired = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
           }
           ~StlTimedMap()
           {
           }
           void insert(Key mapKey,Value val,unsigned int timeOut)
           {
                lock_guard<mutex> lock(this->m_mtx);
                map<Key,Value>::insert(pair<Key,Value>(mapKey,val));
                this->addExpireEvent(mapKey,timeOut);
                this->houseKeeep();
           }
           bool erase(Key key,Value &val)
           {
                lock_guard<mutex> lock(this->m_mtx);
                it = map<Key,Value>::find(key);
                if(it != map<Key,Value>::end())
                {
                    val = it->second;
                    map<Key,Value>::erase(it);
                    return true;
                }
                return false;
           }
           bool erase(Key key)
           {
               lock_guard<mutex> lock(this->m_mtx);
               it = map<Key,Value>::find(key);
               if(it != map<Key,Value>::end())
               {
                   map<Key,Value>::erase(it);
                   return true;
               }
               return false;
           }

           bool find(Key key,Value &val)
           {
                lock_guard<mutex> lock(this->m_mtx);
                it = map<Key,Value>::find(key);
                if(it != map<Key,Value>::end())
                {
                    val = it->second;
                    return true;
                }
                return false;
           }

           void clear()
           {
               lock_guard<mutex> lock(this->m_mtx);
               map<Key,Value>::clear();
           }

           void startGet()
           {
               this->m_mtx.lock();
               it = map<Key,Value>::begin();
           }

           bool getNextElement(Value &val)
           {
                if(it != map<Key,Value>::end())
                {
                    val = it->second;
                    ++it;
                    return true;
                }
                else
                {
                    return false;
                }
           }
           bool getNextElement(Key &key,Value &val)
           {
                if(it != map<Key,Value>::end())
                {
                    key = it->first;
                    val = it->second;
                    ++it;
                    return true;
                }
                else
                {
                    return false;
                }
           }
           void stopGet()
           {
               this->m_mtx.unlock();
           }
           bool removeFirstElement(Value &val)
           {
               lock_guard<mutex> lock(this->m_mtx);
               it = map<Key,Value>::begin();
               if(it != map<Key,Value>::end())
               {
                   val = it->second;
                   map<Key,Value>::erase(it);
                   return true;
               }
               return false;
           }

           bool removeFirstElement(Key &key,Value &val)
           {
               lock_guard<mutex> lock(this->m_mtx);
               it = map<Key,Value>::begin();
               if(it != map<Key,Value>::end())
               {
                   key = it->first;
                   val = it->second;
                   map<Key,Value>::erase(it);
                   return true;
               }
               return false;
           }
           size_t size()
           {
               lock_guard<mutex> lock(this->m_mtx);
               return map<Key,Value>::size();
           }

           void addExpireEvent(Key mapKey,unsigned int timeOut)
           {
                shared_ptr<StlQueue<Key>> pQueue;
                long long int expireTime = (duration_cast<seconds>(system_clock::now().time_since_epoch()).count() + timeOut);
                if(!this->m_ExpireMap.find(expireTime,pQueue))
                {
                    pQueue = make_shared<StlQueue<Key>>();
                    this->m_ExpireMap.insert(expireTime,pQueue);
                }

                if(pQueue.get())
                {
                    pQueue->push(mapKey);
                }
           }

           void houseKeeep()
           {
               shared_ptr<StlQueue<Key>> pQueue;
               long long int currentTime = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
               while(this->m_timeExpired <= currentTime)
               {
                   if(this->m_ExpireMap.erase(this->m_timeExpired,pQueue))
                   {
                       if(pQueue.get())
                       {
                           Key mapKey;
                           while(pQueue->pop(mapKey))
                           {
                                cout<<"Expired ================================="<<mapKey<<endl;
                                map<Key,Value>::erase(mapKey);
                                cout<<"Map Count "<<map<Key,Value>::size()<<endl;
                           }
                           pQueue->clear();
                           //delete pQueue;
                           //pQueue = nullptr;
                       }
                   }
                   this->m_timeExpired++;
               }
           }

           private:
            typename map<Key,Value>::iterator it;
            std::mutex m_mtx;
            long long int m_timeExpired;
            StlMap<long long int,shared_ptr<StlQueue<Key>>> m_ExpireMap;
};


template<class T>
class StlVector:private vector<T>
{
public:
    StlVector()
    {
    }
    ~StlVector()
    {

    }

    void insert(T value)
    {
        lock_guard<mutex> lock(this->m_mtx);
        vector<T>::insert(vector<T>::end(),value);
    }
    void pop_back()
    {
        lock_guard<mutex> lock(this->m_mtx);
        vector<T>::pop_back();
    }

    size_t size()
    {
        lock_guard<std::mutex> lock(this->m_mtx);
        return vector<T>::size();
    }

    void clear()
    {
        lock_guard<mutex> lock(this->m_mtx);
        vector<T>::size();
    }

    T& operator[](size_t index)
    {
        lock_guard<mutex> lock(this->m_mtx);
        return vector<T>::operator[](index);
    }

    bool isExist(const T& searchVal)
    {
        lock_guard<mutex> lock(this->m_mtx);
        if(find(vector<T>::begin(),vector<T>::end(),searchVal) != vector<T>::end())
        {
            return true;
        }
        return false;
    }

private:

    mutex m_mtx;
};
#endif
