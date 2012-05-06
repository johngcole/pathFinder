#include "Stoppable.h"

namespace cole {
namespace thread {

Stoppable::Stoppable() :
	_threads() {
}
Stoppable::~Stoppable() {
	Stop();
}

void Stoppable::Stop() {
	_threads.interrupt_all();
	_threads.join_all();
}

}
}
