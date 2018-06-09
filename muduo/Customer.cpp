#include <map>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

// #include "../Mutex.h"
#include <boost/noncopyable.hpp>
#include <assert.h>
#include <pthread.h>

namespace muduo
{

class MutexLock : boost::noncopyable
{
 public:
  MutexLock()
    : holder_(0)
  {
    pthread_mutex_init(&mutex_, NULL);
  }

  ~MutexLock()
  {
    // assert(holder_ == 0);
    pthread_mutex_destroy(&mutex_);
  }

  // bool isLockedByThisThread()
  // {
  //   return holder_ == CurrentThread::tid();
  // }

  // void assertLocked()
  // {
  //   assert(isLockedByThisThread());
  // }

  // internal usage

  void lock()
  {
    pthread_mutex_lock(&mutex_);
    // holder_ = CurrentThread::tid();
  }

  void unlock()
  {
    holder_ = 0;
    pthread_mutex_unlock(&mutex_);
  }

  pthread_mutex_t* getPthreadMutex() /* non-const */
  {
    return &mutex_;
  }

 private:

  pthread_mutex_t mutex_;
  pid_t holder_;
};

class MutexLockGuard : boost::noncopyable
{
 public:
  explicit MutexLockGuard(MutexLock& mutex) : mutex_(mutex)
  {
    mutex_.lock();
  }

  ~MutexLockGuard()
  {
    mutex_.unlock();
  }

 private:

  MutexLock& mutex_;
};

}





using std::string;

class CustomerData : boost::noncopyable
{
 public:
  CustomerData()
    : data_(new Map)
  { }

  int query(const string& customer, const string& stock) const;

 private:
  typedef std::pair<string, int> Entry;
  typedef std::vector<Entry> EntryList;
  typedef std::map<string, EntryList> Map;
  typedef boost::shared_ptr<Map> MapPtr;
  void update(const string& customer, const EntryList& entries);
  void update(const string& message);

  static int findEntry(const EntryList& entries, const string& stock);
  static MapPtr parseData(const string& message);

  MapPtr getData() const
  {
    muduo::MutexLockGuard lock(mutex_);
    return data_;
  }

  mutable muduo::MutexLock mutex_;
  MapPtr data_;
};

int CustomerData::findEntry(const EntryList& entries, const string& stock)
{
//	if (entries){
//		for (Entry *entry : entries) {
//			if(((entry->first)->compare(stock)) == 0)
//				return 0; 
//		}
//	}
	return -1;
}

int CustomerData::query(const string& customer, const string& stock) const
{
  MapPtr data = getData();

  Map::const_iterator entries = data->find(customer);
  if (entries != data->end())
    return findEntry(entries->second, stock);
  else
    return -1;
}

void CustomerData::update(const string& customer, const EntryList& entries)
{
  muduo::MutexLockGuard lock(mutex_);
  if (!data_.unique())
  {
    MapPtr newData(new Map(*data_));
    data_.swap(newData);
  }
  assert(data_.unique());
  (*data_)[customer] = entries;
}

// void CustomerData::update(const string& message)
// {
//   MapPtr newData = parseData(message);
//   if (newData)
//   {
//     muduo::MutexLockGuard lock(mutex_);
//     data_.swap(newData);
//   }
// }

int main()
{
  CustomerData data;
}
