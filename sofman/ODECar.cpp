#include "ODECar.h"

ODECar::ODECar(Config *config, StatusVariables *status) :
	Stoppable() {
	_config = config;
	_status = status;
	_cmdSpeed = _cmdSteer = 0.0f;
	_speed = _steer = 0.0f;
	_validCmd = false;
}

ODECar::~ODECar() {
}



const dReal *ODECar::getMainBoxPos() {
	_odeMutex.lock();
	const dReal* ans = dGeomGetPosition(_carBox[0]);
	_odeMutex.unlock();
	return ans;
}
const dReal *ODECar::getMainBoxAttitude() {
	_odeMutex.lock();
	const dReal* ans = dGeomGetRotation(_carBox[0]);
	_odeMutex.unlock();
	return ans;
}
void ODECar::getMainBoxLengths(dVector3 sides) {
	_odeMutex.lock();
	dGeomBoxGetLengths(_carBox[0],sides);
	_odeMutex.unlock();
}
const dReal *ODECar::getSmallBoxPos() {
	_odeMutex.lock();
	const dReal* ans = dGeomGetPosition(_carBox[1]);
	_odeMutex.unlock();
	return ans;
}
const dReal *ODECar::getSmallBoxAttitude() {
	_odeMutex.lock();
	const dReal* ans = dGeomGetRotation(_carBox[0]);
	_odeMutex.unlock();
	return ans;
}
void ODECar::getSmallBoxLengths(dVector3 sides) {
	_odeMutex.lock();
	dGeomBoxGetLengths(_carBox[1],sides);
	_odeMutex.unlock();
}
const dReal *ODECar::getWheelPos(int wheel) {
	_odeMutex.lock();
	const dReal* ans = dGeomGetPosition(_carSphere[wheel]);
	_odeMutex.unlock();
	return ans;
}
const dReal *ODECar::getWheelAttitude(int wheel) {
	_odeMutex.lock();
	const dReal* ans = dGeomGetRotation(_carSphere[wheel]);
	_odeMutex.unlock();
	return ans;
}
float ODECar::getWheelRadius() {
	return RADIUS;
}





float ODECar::getCarSpeed() {
	_odeMutex.lock();
	float f = _speed;
	_odeMutex.unlock();
	return f;
}
float ODECar::getCarSteering() {
	_odeMutex.lock();
	float f = _steer;
	_odeMutex.unlock();
	return f;
}
void ODECar::setCarSpeed(float speed) {
	_odeMutex.lock();
	_cmdSpeed = MIN( MAX( speed, 0.0f ), 1.0f );
	_validCmd = true;
	_odeMutex.unlock();
}
void ODECar::setCarSteering(float steer) {
	_odeMutex.lock();
	_cmdSteer = MIN( MAX( steer, -1.0f ), 1.0f );
	_validCmd = true;
	_odeMutex.unlock();
}

void ODECar::Start() {
	startThread(ODECar::ODEThread, this);
}

void ODECar::ODEThread(void *arg) {
	ODECar *ode = (ODECar*) arg;
	int i, bodyI = 0, boxI = 0, jointI = 0, sphI = 0;
	double x = 0.0, y = 0.0, z = STARTZ;
	float desiredSpeed, desiredSteer;
	dMass m;
	PathError pe;

	ode->_odeMutex.lock();
	desiredSpeed = ode->_cmdSpeed;
	desiredSteer = ode->_cmdSteer;
	dInitODE2(0);
	ode->_world = dWorldCreate();
	ode->_space = dHashSpaceCreate(0);
	ode->_contactgroup = dJointGroupCreate(0);
	dWorldSetGravity(ode->_world, 0, 0, -0.5);
	ode->_ground = dCreatePlane(ode->_space, 0, 0, 1, 0);
	dAllocateODEDataForThread( dAllocateMaskAll);

	//	c1 = new Car(LENGTH,WIDTH,HEIGHT,RADIUS);
	//	c1->makeCar(world,0.0, 0.0,STARTZ);

	// chassis body
	ode->_carBody[bodyI] = dBodyCreate(ode->_world);
	dBodySetPosition(ode->_carBody[bodyI], x, y, z);
	dMassSetBox(&m, 1, LENGTH, WIDTH, HEIGHT);
	dMassAdjust(&m, 0.5);
	dBodySetMass(ode->_carBody[bodyI], &m);
	// make in space 0 then build together into carspace and assign it to space to make sim faster
	ode->_carBox[boxI] = dCreateBox(0, LENGTH, WIDTH, HEIGHT);
	dGeomSetBody(ode->_carBox[boxI], ode->_carBody[bodyI]);

	// wheel bodies
	for (i = 1; i <= 4; i++) {
		ode->_carBody[bodyI + i] = dBodyCreate(ode->_world);
		dQuaternion q;
		dQFromAxisAndAngle(q, 1, 0, 0, M_PI * 0.5);
		dBodySetQuaternion(ode->_carBody[bodyI + i], q);
		dMassSetSphere(&m, 1, RADIUS);
		dMassAdjust(&m, WMASS);
		dBodySetMass(ode->_carBody[bodyI + i], &m);
		ode->_carSphere[sphI + i - 1] = dCreateSphere(0, RADIUS);
		dGeomSetBody(ode->_carSphere[sphI + i - 1], ode->_carBody[bodyI + i]);
	}
	// body 1-2 front wheels 3-4 back wheels
	//  push front wheels a little farther forward
	dBodySetPosition(ode->_carBody[bodyI + 1],
			x + 0.5 * LENGTH - 0.25 * RADIUS, y + WIDTH * 0.5, z - HEIGHT * 0.5);
	dBodySetPosition(ode->_carBody[bodyI + 2],
			x + 0.5 * LENGTH - 0.25 * RADIUS, y - WIDTH * 0.5, z - HEIGHT * 0.5);
	dBodySetPosition(ode->_carBody[bodyI + 3], x - 0.4 * LENGTH + 0.5 * RADIUS,
			y + WIDTH * 0.5, z - HEIGHT * 0.5);
	dBodySetPosition(ode->_carBody[bodyI + 4], x - 0.4 * LENGTH + 0.5 * RADIUS,
			y - WIDTH * 0.5, z - HEIGHT * 0.5);

	// front and back wheel hinges
	for (i = 0; i < 4; i++) {
		ode->_carJoint[jointI + i] = dJointCreateHinge2(ode->_world, 0);
		dJointAttach(ode->_carJoint[jointI + i], ode->_carBody[bodyI],
				ode->_carBody[bodyI + i + 1]);
		const dReal *a = dBodyGetPosition(ode->_carBody[bodyI + i + 1]);
		dJointSetHinge2Anchor(ode->_carJoint[jointI + i], a[0], a[1], a[2]);
		dJointSetHinge2Axis1(ode->_carJoint[jointI + i], 0, 0, (i < 2 ? 1 : -1));
		dJointSetHinge2Axis2(ode->_carJoint[jointI + i], 0, 1, 0);
		dJointSetHinge2Param(ode->_carJoint[jointI + i], dParamSuspensionERP,
				0.4);
		dJointSetHinge2Param(ode->_carJoint[jointI + i], dParamSuspensionCFM,
				0.8);
		if (i < 2) {
			dJointSetHinge2Param(ode->_carJoint[jointI + i], dParamVel2, 0);
			dJointSetHinge2Param(ode->_carJoint[jointI + i], dParamFMax2, FMAX);
			dJointSetHinge2Param(ode->_carJoint[jointI + i], dParamStopERP,
					0.99f);
			dJointSetHinge2Param(ode->_carJoint[jointI + i], dParamStopCFM,
					0.001f);
		}
		// lock wheels along the steering axis
		dJointSetHinge2Param(ode->_carJoint[i], dParamLoStop, 0);
		dJointSetHinge2Param(ode->_carJoint[i], dParamHiStop, 0);
	}

	// create a small black box on top
	ode->_carBody[bodyI + 5] = dBodyCreate(ode->_world);
	dBodySetPosition(ode->_carBody[bodyI + 5], x, y, z + HEIGHT * 0.6);
	dMassSetBox(&m, 1, LENGTH * 0.5, WIDTH * 0.6, HEIGHT * 0.6);
	dMassAdjust(&m, 0.5);
	dBodySetMass(ode->_carBody[bodyI + 5], &m);
	dJointID j = dJointCreateFixed(ode->_world, 0);
	dJointAttach(j, ode->_carBody[bodyI], ode->_carBody[bodyI + 5]);
	dJointSetFixed(j);
	ode->_carBox[boxI + 1] = dCreateBox(0, LENGTH * 0.5, WIDTH * 0.6, HEIGHT
			* 0.6);
	dGeomSetBody(ode->_carBox[boxI + 1], ode->_carBody[bodyI + 5]);

	ode->_carspace = dSimpleSpaceCreate(ode->_space);
	dSpaceSetCleanup(ode->_carspace, 0);
	for (i = 0; i < 2; i++)
		dSpaceAdd(ode->_carspace, ode->_carBox[i]);
	for (i = 0; i < 4; i++)
		dSpaceAdd(ode->_carspace, ode->_carSphere[i]);
	ode->_odeMutex.unlock();

	////////////////
	///////////// SIM LOOP
	/////////////////
	try {
		while (true) {
			boost::this_thread::interruption_point();

			ode->_odeMutex.lock();
			if (ode->_validCmd) {
				desiredSpeed = ((int)(ode->_cmdSpeed*1000.0f)) / 1000.f;
				desiredSteer = ((int)(ode->_cmdSteer*1000.0f)) / 1000.f;
				ode->_validCmd = false;
			}
			if (abs(desiredSpeed-ode->_speed) < 0.00001)
				desiredSpeed = ode->_speed;
			if (ode->_speed < desiredSpeed)
				ode->_speed += 0.001f;
			if (ode->_speed > desiredSpeed)
				ode->_speed -= 0.001f;
			if (abs(desiredSteer-ode->_steer) < 0.00001)
				desiredSteer = ode->_steer;
			if (ode->_steer < desiredSteer)
				ode->_steer += 0.001f;
			if (ode->_steer > desiredSteer)
				ode->_steer -= 0.001f;

			//motor
			dJointSetHinge2Param(ode->_carJoint[0], dParamVel2, -ode->_speed);
			dJointSetHinge2Param(ode->_carJoint[0], dParamFMax2, 0.1);
			dJointSetHinge2Param(ode->_carJoint[1], dParamVel2, -ode->_speed);
			dJointSetHinge2Param(ode->_carJoint[1], dParamFMax2, 0.1);
			//steering
			dReal v = ode->_steer - dJointGetHinge2Angle1(ode->_carJoint[0]);
			if (v > 0.1)
				v = 0.1;
			if (v < -0.1)
				v = -0.1;
			v *= 10.0;
			dJointSetHinge2Param(ode->_carJoint[0], dParamVel, v);
			dJointSetHinge2Param(ode->_carJoint[0], dParamFMax, 0.2);
			dJointSetHinge2Param(ode->_carJoint[0], dParamLoStop, -0.75);
			dJointSetHinge2Param(ode->_carJoint[0], dParamHiStop, 0.75);
			dJointSetHinge2Param(ode->_carJoint[0], dParamFudgeFactor, 0.1);
			dJointSetHinge2Param(ode->_carJoint[1], dParamVel, v);
			dJointSetHinge2Param(ode->_carJoint[1], dParamFMax, 0.2);
			dJointSetHinge2Param(ode->_carJoint[1], dParamLoStop, -0.75);
			dJointSetHinge2Param(ode->_carJoint[1], dParamHiStop, 0.75);
			dJointSetHinge2Param(ode->_carJoint[1], dParamFudgeFactor, 0.1);

			dSpaceCollide(ode->_space, (void*) ode, &nearCallback);

			dWorldStep(ode->_world, 0.05);
			// remove all contact joints
			dJointGroupEmpty(ode->_contactgroup);

			// post position to status
			const dReal* pos = dGeomGetPosition(ode->_carBox[0]);
			Length x( pos[0], Length::METERS );
			Length y( pos[1], Length::METERS );
			Length z( pos[2], Length::METERS );
			Position3D position(x,y,z);
			//Logger::getInstance()->log("Position: " + position.toString());
			// get the old position first to allow update to distance traveled
			Position3D oldpos = ode->_status->getCarPosition();
			ode->_status->setCarPosition( position );

			const dReal* R = dGeomGetRotation(ode->_carBox[0]);
			NorthBearingAngle yaw = Angle(atan2(R[4], R[0]), Angle::RADIANS).toNorthBearing();
			double p = atan2(-R[8], sqrt(R[9]*R[9] + R[10]*R[10]));
			Angle pitch( p, Angle::RADIANS );
			Angle roll( atan2(R[9], R[10]), Angle::RADIANS );
			Attitude att( roll, pitch, yaw );
			//Logger::getInstance()->log(att.toString());
			ode->_status->setCarAttitude(att);

			// update the path error in status
			ode->_status->getPath()->fillPathError(position, att.getYaw(), pe);
			ode->_status->updateStats(pe.DistanceError,
						  position.getGroundRangeTo(oldpos));

			ode->_odeMutex.unlock();
			boost::this_thread::sleep(boost::posix_time::milliseconds(1));
		}
	} catch (boost::thread_interrupted const&) {
		// exiting ode thread
		Logger::getInstance()->log("ODE Car Thread exiting...");
	}
}

void ODECar::nearCallback(void *data, dGeomID o1, dGeomID o2) {
	int i, n;
	const int N = 10;
	dContact contact[N];
	ODECar *ode = (ODECar*) data;

	// we want all collisions
	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);
	if (b1 && b2 && dAreConnected(b1, b2))
		return;

	n = dCollide(o1, o2, N, &contact[0].geom, sizeof(dContact));
	if (n > 0) {
		for (i = 0; i < n; i++) {
			contact[i].surface.mode = dContactSlip1 | dContactSlip2
					| dContactSoftERP | dContactSoftCFM | dContactApprox1;

			// jgc adj values -- see contact in manual for descrip
			contact[i].surface.mu = 0.95;
			contact[i].surface.slip1 = 0.05;
			contact[i].surface.slip2 = 0.05;
			contact[i].surface.soft_erp = 0.5;
			contact[i].surface.soft_cfm = 0.01;

			dJointID c = dJointCreateContact(ode->_world, ode->_contactgroup,
					&contact[i]);
			//dJointID c = dJointCreateContact (world,contactgroup,contact+i);
			dJointAttach(c, dGeomGetBody(contact[i].geom.g1), dGeomGetBody(
					contact[i].geom.g2));
		}
	}
}

