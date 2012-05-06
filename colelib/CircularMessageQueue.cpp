#include "CircularMessageQueue.h"

namespace cole {
namespace data {

CircularMessageQueue::CircularMessageQueue(const string strID) :
	_id(strID), _buffer(NUM_ENTRIES_CIRC_MSG_QUEUE), _Qmutex() {
}

CircularMessageQueue::~CircularMessageQueue() {
}

string CircularMessageQueue::getID() {
	return _id;
}

void CircularMessageQueue::clear() {
	_Qmutex.lock();
	_buffer.clear();
	_Qmutex.unlock();
}

void CircularMessageQueue::push(const vector<unsigned char> &message) {
	_Qmutex.lock();
	_buffer.push_back(message);
	_Qmutex.unlock();
}

void CircularMessageQueue::push(const string &message) {
	vector<unsigned char> vec;
	BOOST_FOREACH( char c, message )
{	vec.push_back( (unsigned char)c );
}
push(vec);
}

vector<unsigned char> CircularMessageQueue::pop() {
vector<unsigned char> ans;
_Qmutex.lock();
if (_buffer.size() > 0) {
	ans = _buffer.front();
	_buffer.pop_front();
}
_Qmutex.unlock();
return ans;
}

}
}

