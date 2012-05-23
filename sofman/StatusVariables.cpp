#include "StatusVariables.h"

StatusVariables::StatusVariables() :
_carMutex(),
_carPos(Position3D::ZERO),
_carAtt(Attitude::INVALID_ATT)
{
}
StatusVariables::~StatusVariables() {
}

void StatusVariables::setCarPosition(Position3D &pos) {
	_carMutex.lock();
	_carPos = pos;
	_carMutex.unlock();
}
void StatusVariables::setCarAttitude(Attitude &att) {
	_carMutex.lock();
	_carAtt = att;
	_carMutex.unlock();
}

void StatusVariables::getCarPosition(Position3D &pos) {
	_carMutex.lock();
	pos = _carPos;
	_carMutex.unlock();
}
void StatusVariables::getCarAttitude(Attitude &att) {
	_carMutex.lock();
	att = _carAtt;
	_carMutex.unlock();
}
