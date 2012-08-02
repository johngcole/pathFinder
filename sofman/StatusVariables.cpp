#include "StatusVariables.h"

StatusVariables::StatusVariables() :
_carMutex(),
_carPos(Position3D::ZERO),
_carAtt(Attitude::INVALID_ATT),
_pathMutex()
{
	_path.reset();
	_sumErrorSquares = Length(0.0,Length::METERS);
	_sumDistanceTraveled = Length(0.0,Length::METERS);
	_errorMeasureCount = 0;
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

void StatusVariables::updateStats(Length errv, Length dist) {
	_statsMutex.lock();
	_sumErrorSquares = _sumErrorSquares.plus(Length(pow(errv.getDoubleValue(Length::METERS),2.0),Length::METERS));
	_sumDistanceTraveled = _sumDistanceTraveled.plus(dist);
	_errorMeasureCount++;
	_statsMutex.unlock();
}


void StatusVariables::startStats() {
	_statsMutex.lock();
	_startTime = microsec_clock::local_time();
	_statsMutex.unlock();
}

ptime StatusVariables::getStartTime() {
	_statsMutex.lock();
	ptime pt = _startTime;
	_statsMutex.unlock();
	return pt;
}

Length StatusVariables::getErrorValue() {
	_statsMutex.lock();
	Length err = Length(_sumErrorSquares);
	_statsMutex.unlock();
	return err;
}

Length StatusVariables::getDistanceTraveled() {
	_statsMutex.lock();
	Length dist = _sumDistanceTraveled;
	_statsMutex.unlock();
	return dist;
}

int StatusVariables::getMeasurementCount() {
	_statsMutex.lock();
	int cnt = _errorMeasureCount;
	_statsMutex.unlock();
	return cnt;
}

double StatusVariables::getTripStandardError() {
	_statsMutex.lock();
	double err = sqrt(_sumErrorSquares.getDoubleValue(Length::METERS)) /sqrt(_errorMeasureCount);
	_statsMutex.unlock();
	return err;
}
	
