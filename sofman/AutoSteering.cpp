#include "AutoSteering.h"

AutoSteering::AutoSteering(ODECar *car, StatusVariables *status, int periodMs) {
	_car = car;
	_status = status;
	_waitTimeMs = periodMs;
}

void AutoSteering::Start() {
	startThread(AutoSteering::AutoSteerThread, this);
}

void AutoSteering::AutoSteerThread(void *arg) {
	AutoSteering *as = (AutoSteering*) arg;
	PathError err;
	float speed, steer;

	try {
		while (true) {
			boost::this_thread::interruption_point();

			Position3D pos = as->_status->getCarPosition();
			Attitude att = as->_status->getCarAttitude();
			boost::shared_ptr<Path> path = as->_status->getPath();
			path->fillPathError(pos, att.getYaw(), err);
			as->fillCommands(err, &speed, &steer);

			as->_car->setCarSpeed(speed);
			as->_car->setCarSteering(steer);

			boost::this_thread::sleep(boost::posix_time::milliseconds(as->_waitTimeMs));
		}
	} catch (boost::thread_interrupted const&) {
		Logger::getInstance()->log("AutoSteering Thread exiting...");
	}

}
