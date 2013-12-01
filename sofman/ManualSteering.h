/*
 * ManualSteering.h
 *
 *  Created on: May 27, 2012
 *      Author: gray
 */

#ifndef __MANUALSTEERING_H_
#define __MANUALSTEERING_H_

#include <iostream>

#include <colelib/Constants.h>
#include <colelib/Logger.h>

#include "KeyListener.h"
#include "ODECar.h"

using namespace cole::util;
using namespace std;

class ManualSteering : public KeyListener {
public:
	ManualSteering(ODECar *car);
	virtual ~ManualSteering();

	void keyUp(short key);
	void keyDown(short key);

private:
	ODECar *_car;
};

#endif /* MANUALSTEERING_H_ */
