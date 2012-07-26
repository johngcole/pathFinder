#include "StatusVariables.h"

StatusVariables::StatusVariables() :
_carMutex(),
_carPos(Position3D::ZERO),
_carAtt(Attitude::INVALID_ATT),
_pathMutex()
{
	_path.reset();
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

Position3D StatusVariables::getCarPosition() {
	_carMutex.lock();
	Position3D pos = _carPos;
	_carMutex.unlock();
	return pos;
}
Attitude StatusVariables::getCarAttitude() {
	_carMutex.lock();
	Attitude att = _carAtt;
	_carMutex.unlock();
	return att;
}

void StatusVariables::setPath(boost::shared_ptr<Path> path) {
	_pathMutex.lock();
	_path = path;
	_pathMutex.unlock();
}

boost::shared_ptr<Path> StatusVariables::getPath() {
	_pathMutex.lock();
	boost::shared_ptr<Path> pathPtr = _path;
	_pathMutex.unlock();
	return pathPtr;
}



