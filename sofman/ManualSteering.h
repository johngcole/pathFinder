/*
 * ManualSteering.h
 *
 *  Created on: May 27, 2012
 *      Author: gray
 */

#ifndef __MANUALSTEERING_H_
#define __MANUALSTEERING_H_

#include <boost/thread/mutex.hpp>

#include <iostream>

#include <colelib/Constants.h>
#include <colelib/Logger.h>

#include "KeyListener.h"
#include "ODECar.h"

using namespace cole::util;
using namespace std;

class ManualSteering : public KeyListener, public Stoppable {
public:
	ManualSteering(ODECar *car);
	~ManualSteering();

	void keyUp(short key);
	void keyDown(short key);

	void Start();

private:
	static void SteerThread(void *arg);

	ODECar *_car;
	int _command;
	bool _left, _right;
	boost::mutex _mutex;
};

#endif /* MANUALSTEERING_H_ */
