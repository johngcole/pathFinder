#ifndef __CIRCULAR_QUEUE_H__
#define __CIRCULAR_QUEUE_H__

#include <boost/foreach.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/circular_buffer.hpp>
#include <vector>

using namespace std;

namespace cole {
namespace data {

#define NUM_ENTRIES_CIRC_MSG_QUEUE 50

class CircularMessageQueue {

public:
	CircularMessageQueue(string id);
	~CircularMessageQueue();
	string getID();
	void push(const vector<unsigned char> &message);
	void push(const string &message);
	vector<unsigned char> pop();
	void clear();

private:
	string _id;
	boost::circular_buffer<vector<unsigned char> > _buffer;
	boost::mutex _Qmutex;
};

}
}

#endif /* CIRCULAR_QUEUE_H */
