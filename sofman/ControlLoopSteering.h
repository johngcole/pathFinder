/*
 * ControlLoopSteering.h
 *
 *  Created on: Aug 11, 2012
 *      Author: gray
 */

#ifndef CONTROLLOOPSTEERING_H_
#define CONTROLLOOPSTEERING_H_

#include <iostream>
#include <colelib/Constants.h>
#include <colelib/Logger.h>

#include "AutoSteering.h"

using namespace cole::util;
using namespace std;

class ControlLoopSteering : public AutoSteering {
public:
	ControlLoopSteering(ODECar *car, StatusVariables *status, Config *config, int periodMs);
	~ControlLoopSteering() {}

protected:
	void fillCommands(PathError &err, float *speed, float *steering);

private:
	double computeError( PathError &err );

	double K_p, K_i, K_d;
	double _maxSpeed;
	double _e_last, _e_sum;
};

#endif /* CONTROLLOOPSTEERING_H_ */
