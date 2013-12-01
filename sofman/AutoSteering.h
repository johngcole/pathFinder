
#ifndef __AUTOSTEERING_H_
#define __AUTOSTEERING_H_

#include <boost/shared_ptr.hpp>

#include <iostream>
#include <colelib/Constants.h>
#include <colelib/Logger.h>
#include <colelib/Stoppable.h>

#include "ODECar.h"
#include "StatusVariables.h"
#include "Path.h"

using namespace cole::util;
using namespace cole::thread;
using namespace std;

class AutoSteering : public Stoppable {
public:
	AutoSteering(ODECar *car, StatusVariables *status, int periodMs);
	virtual ~AutoSteering() {}

	virtual void Start();

protected:
	virtual void fillCommands(PathError &err, float *speed, float *steering) = 0;

	int _waitTimeMs;
private:
	static void AutoSteerThread(void *arg);
	ODECar *_car;
	StatusVariables *_status;
};

#endif /* MANUALSTEERING_H_ */
