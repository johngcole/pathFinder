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

	ostringstream oss;
	float speed;
	unsigned char k = (unsigned char)(key & 0x00FF);
	switch (k)
	{
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case '0':
		speed = (float)(k - '0') / 10.0f;
		oss << ">>> Speed -> " << speed;
		Logger::getInstance()->log(oss.str().c_str());
		_car->setCarSpeed(speed);
		break;
	case 'q':
		_car->setCarSteering(-1.0f);
		break;
	case 'p':
		_car->setCarSteering(1.0f);
		break;
	}
}


