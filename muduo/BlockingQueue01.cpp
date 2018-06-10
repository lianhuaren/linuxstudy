
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
		//assert(holder_ == 0);
		pthread_mutex_destroy(&mutex_);
	}

	void lock()
	{
		pthread_mutex_lock(&mutex_);
		assignHolder();
	}

	void unlock()
	{
		unassignHolder();
		pthread_mutext_unlock(&mutext_);
	}

private:
	void unassignHolder()
	{
		holder_ = 0;
	}

	void assignHolder()
	{
		// holder_ = CurrentThread::tid();
	}

	pthread_mutex_t mutex_;
	pid_t holder_;
};

}






int main() {


    return 0;
}