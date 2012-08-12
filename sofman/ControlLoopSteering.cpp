#include "ControlLoopSteering.h"


ControlLoopSteering::ControlLoopSteering(ODECar *car, StatusVariables *status, Config *config, int periodMs) :
AutoSteering(car, status, periodMs) {
	K_p = config->getValueAsDouble("ControlLoop_Kp");
	K_i = config->getValueAsDouble("ControlLoop_Ki");
	K_d = config->getValueAsDouble("ControlLoop_Kd");

	_e_last = 0.0;
	_e_sum = 0.0;
}

double ControlLoopSteering::computeError( PathError &err ) {

	// Reminder:
	// err.DistanceError - negative values indicate areas left of path
	// err.BearingError - negative values indicate headings left of path's heading

	double dist = -1 * err.DistanceError.getDoubleValue(Length::METERS) / 10.0;
	double bear = -1 * err.BearingError.getDoubleValue(Angle::DEGREES) / 45.0;

	return dist + 0.5*bear;
}

void ControlLoopSteering::fillCommands(PathError &err, float *speed, float *steering) {

	double e = computeError(err);

	double P = K_p * e;

	_e_sum += e;
	double I = K_i * _e_sum;

	double D = K_d * ((e - _e_last) / (_waitTimeMs/1000.0));
	_e_last = e;

	*steering = P + I + D;
	double steer = MIN( MAX( *steering, -0.5f ), 0.5f );
	*speed = (0.5 - abs(steer)) * 2.0;
	*speed = MAX( *speed, 0.1 );

	ostringstream oss;
	oss << "ControlLoop: Steer->" << *steering << " Speed->" << *speed;
	Logger::getInstance()->log(oss.str());
}
