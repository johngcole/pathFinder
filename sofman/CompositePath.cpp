#include "CompositePath.h"

CompositePath::CompositePath(vector<boost::shared_ptr<Path> > paths) :
	_paths() {
	_paths = paths;
	if (_paths.size() == 0)
		Logger::getInstance()->log("WARNING: empty CompositePath...");
}

Length CompositePath::length() {
	boost::shared_ptr<Path> path;
	Length tot(Length::ZERO);
	BOOST_FOREACH(path, _paths) {
		tot = tot.plus(path->length());
	}
	return tot;
}

Position3D CompositePath::start() {
	if (_paths.size() == 0)
		return Position3D();
	else
		return _paths[0]->start();
}

Position3D CompositePath::end() {
	if (_paths.size() == 0)
		return Position3D();
	else
		return _paths[_paths.size() - 1]->end();
}

NorthBearingAngle CompositePath::startBearing() {
	if (_paths.size() == 0)
		return NorthBearingAngle::NORTH;
	else
		return _paths[0]->startBearing();
}

NorthBearingAngle CompositePath::endBearing() {
	if (_paths.size() == 0)
		return NorthBearingAngle::NORTH;
	else
		return _paths[_paths.size() - 1]->endBearing();
}

Position3D CompositePath::findPosition(float percentage) {
	Length dist = length().scale(percentage);
	return findPosition(dist);
}

Position3D CompositePath::findPosition(Length dist) {
	Length curr(dist), temp;
	int i = 0;
	while (true) {
		if (i > (int) _paths.size())
			return Position3D();

		temp = curr.minus(_paths[i]->length());
		if (temp < Length::ZERO)
			return _paths[i]->findPosition(curr);
		curr = temp;
		i++;
	}
}

void CompositePath::fillPathError(Position3D currPos,
		NorthBearingAngle currHeading, PathError &error) {

	PathError bestFit, curr;
	bestFit.DistanceError = Length(DBL_MAX, Length::METERS);
	boost::shared_ptr<Path> path;
	BOOST_FOREACH(path, _paths) {
		path->fillPathError(currPos, currHeading, curr);
		if (curr.DistanceError.abs() < bestFit.DistanceError.abs())
			memcpy(&bestFit, &curr, sizeof(PathError));
	}
	memcpy(&error, &bestFit, sizeof(PathError));
}
