/*
 * ManualSteering.cpp
 *
 *  Created on: May 27, 2012
 *      Author: gray
 */

#include "ManualSteering.h"

ManualSteering::ManualSteering(ODECar *car) {
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
	case 'p':
		_car->setCarSteering(0.0f);
		break;
	}
}
void ManualSteering::keyDown(short key) {
	if (key<0)
		return;

	switch ((unsigned char)(key & 0x00FF))
	{
	case 'q':
		_car->setCarSteering(-1.0f);
		break;
	case 'p':
		_car->setCarSteering(1.0f);
		break;
	}
}


