#include "Stoppable.h"

namespace cole {
namespace thread {

Stoppable::Stoppable() :
	_threads() {
}
Stoppable::~Stoppable() {
	Stop();
}

void Stoppable::startThread( void func(void*), void* arg ) {
	_threads.add_thread(new boost::thread(func, arg));
}

void Stoppable::Stop() {
	_threads.interrupt_all();
	_threads.join_all();
}

void Stoppable::Join() {
	_threads.join_all();
}

}
}
