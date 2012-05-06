// Stoppable.h
#ifndef __STOPPABLE_H__
#define __STOPPABLE_H__

#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <iostream>
using namespace std;

namespace cole {
namespace thread {

class Stoppable {
public:
	Stoppable();
	~Stoppable();

	virtual void Start() = 0;
	void Stop();
	void Join();

protected:
	void startThread( void func(void*), void* arg );

private:
	boost::thread_group _threads;
};

} // namespace thread
} // namespace ssp

#endif

