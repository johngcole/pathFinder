#ifndef __ODECAR_H_
#define __ODECAR_H_

#include <boost/thread/mutex.hpp>

#include <ode/ode.h>
#include <colelib/Stoppable.h>
#include <colelib/Config.h>
#include <colelib/Constants.h>
#include "StatusVariables.h"

using namespace cole::thread;
using namespace cole::util;

#define LENGTH 0.7	// chassis length
#define WIDTH 0.5	// chassis width
#define HEIGHT 0.2	// chassis height
#define RADIUS 0.18	// wheel radius
#define STARTZ 0.5	// starting height of chassis
#define WMASS 0.2
#define FMAX 25

#define MAXBODYPARTS 3

class ODECar: public Stoppable {
public:
	ODECar(Config *config, StatusVariables *status);
	virtual ~ODECar();

	// Stoppable interface
	void Start();

	const dReal *getMainBoxPos();
	const dReal *getMainBoxAttitude();
	void getMainBoxLengths(dVector3 sides);
	const dReal *getSmallBoxPos();
	const dReal *getSmallBoxAttitude();
	void getSmallBoxLengths(dVector3 sides);
	const dReal *getWheelPos(int wheel);
	const dReal *getWheelAttitude(int wheel);
	float getWheelRadius();

	float getCarSpeed();
	float getCarSteering();
	void setCarSpeed(float speed);
	void setCarSteering(float steer);

protected:
	Config *_config;
	StatusVariables *_status;

private:
	static void ODEThread(void *arg);
	static void nearCallback(void *data, dGeomID o1, dGeomID o2);

	boost::mutex _odeMutex;
	dWorldID _world;
	dSpaceID _space, _carspace;
	dJointGroupID _contactgroup;
	dGeomID _ground;
	dBodyID _carBody[MAXBODYPARTS + 4];
	dJointID _carJoint[4];
	dGeomID _carBox[MAXBODYPARTS];
	dGeomID _carSphere[4];
	float _steer, _speed;
	float _cmdSteer, _cmdSpeed;
	bool _validCmd;
};

#endif /* __ODECAR_H_ */
