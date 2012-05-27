/*
 * ManualSteering.cpp
 *
 *  Created on: May 27, 2012
 *      Author: gray
 */

#include "ManualSteering.h"

ManualSteering::ManualSteering(ODECar *car) :
	Stoppable(), _mutex() {
	_command = 0;
	_left = _right = false;
	_car = car;
}

ManualSteering::~ManualSteering() {
}

void ManualSteering::keyUp(short key) {
	if (key<0)
		return;

	switch ((unsigned char)(key & 0x00FF))
	{
	case 'q':
		_mutex.lock();
		_left = false;
		_mutex.unlock();
		break;
	case 'p':
		_mutex.lock();
		_right = false;
		_mutex.unlock();
		break;
	}
}
void ManualSteering::keyDown(short key) {
	if (key<0)
		return;

	switch ((unsigned char)(key & 0x00FF))
	{
	case 'q':
		_mutex.lock();
		_left = true;
		_mutex.unlock();
		break;
	case 'p':
		_mutex.lock();
		_right = true;
		_mutex.unlock();
		break;
	}
}

void ManualSteering::Start() {
	startThread(ManualSteering::SteerThread, this);
}
void ManualSteering::SteerThread(void *arg) {
	ManualSteering *ms = (ManualSteering*)arg;
	float cmd;

	try {
		while (true) {

			ms->_mutex.lock();
			if (ms->_left && !ms->_right)
				ms->_command--;
			else if (ms->_right && !ms->_left)
				ms->_command++;
			else {
				if (ms->_command > 0)
					ms->_command--;
				if (ms->_command < 0)
					ms->_command++;
			}
			ms->_command = MAX( MIN( ms->_command, 10 ), -10 );
			cmd = (float)ms->_command / 10.0f;
			ms->_mutex.unlock();

			ms->_car->setCarSteering(cmd);

			boost::this_thread::sleep(boost::posix_time::milliseconds(100));
		}
	} catch (boost::thread_interrupted const&) {
		Logger::getInstance()->log("Manual Steering Thread exiting...");
	}
}
