#include "StraightPath.h"

StraightPath::StraightPath(Position3D start, Position3D end) {
	_start = start;
	_end = end;
	_path_bearing = _start.getBearingTo(_end);
}
StraightPath::StraightPath(Position3D start, NorthBearingAngle bearing,
		Length len) {
	_start = start;
	_path_bearing = bearing;
	_end = _start.move(bearing, len);
}

Length StraightPath::length() {
	return _start.getGroundRangeTo(_end);
}
Position3D StraightPath::start() {
	return _start;
}
Position3D StraightPath::end() {
	return _end;
}
NorthBearingAngle StraightPath::startBearing() {
	return _path_bearing;
}
NorthBearingAngle StraightPath::endBearing() {
	return _path_bearing;
}

Position3D StraightPath::findPosition(Length dist) {
	if (dist <= Length::ZERO)
		return _start;
	if (dist >= length())
		return _end;

	return _start.move(_path_bearing, dist);
}

Position3D StraightPath::findPosition(float percentage)  {
	Length dist = length().scale(percentage);
	return findPosition(dist);
}

bool StraightPath::isLeft( Position3D a, Position3D b, Position3D c ) {
	double bax = b.getX().minus(a.getX()).getDoubleValue(Length::METERS);
	double cay = c.getY().minus(a.getY()).getDoubleValue(Length::METERS);
	double bay = b.getY().minus(a.getY()).getDoubleValue(Length::METERS);
	double cax = c.getX().minus(a.getX()).getDoubleValue(Length::METERS);

	return (bax*cay - bay*cax) > 0;
}

bool StraightPath::insideStraightPath(Position3D pos) {

	NorthBearingAngle right = _path_bearing.plus(Angle::RIGHT_ANGLE);
	NorthBearingAngle left = _path_bearing.minus(Angle::RIGHT_ANGLE);

	Position3D sls = _start.move(right, Length::ONE_METER);
	Position3D slf = _start.move(left, Length::ONE_METER);
	bool leftOfStart = isLeft(sls, slf, pos);

	Position3D els = _end.move(right, Length::ONE_METER);
	Position3D elf = _end.move(left, Length::ONE_METER);
	bool leftOfEnd = isLeft(els, elf, pos);

	return (!leftOfStart && leftOfEnd);
}

void StraightPath::fillPathError(Position3D currPos,
		NorthBearingAngle currHeading, PathError &error) {

	Position3D closestPoint;
	if (!insideStraightPath(currPos)) {
		// the point is outside the perpindicular plane of the straight path
		//     closest point is just closer between start and end
		Length toEnd = currPos.getGroundRangeTo(_end);
		Length toStart = currPos.getGroundRangeTo(_start);
		closestPoint = (toEnd <= toStart) ? _end : _start;
	} else {
		// ok, now we can do our intersect
		Vector3D end_vec = _start.getVectorTo(_end);
		Vector3D spot_vec = _start.getVectorTo(currPos);

		// first need to check for simple cases
		if (end_vec.getX() == Length::ZERO) {
			// this is a north-south vector
			closestPoint = Position3D(_start.getX(), currPos.getY(), Length::ZERO);
		} else if (end_vec.getY() == Length::ZERO) {
			// this is a east-west vector
			closestPoint = Position3D(currPos.getX(), _start.getY(), Length::ZERO);
		} else {
			// going to solve for intersection of two lines:
			//    line 1: the path
			//    line 2: perpindicular line to path, going through currPos

			// line 1:   y - y_s = m1 * (x - x_s)
			//                m1 = (y_s - y_e) / (x_s - x_e)
			//        in this case, x_s = y_s = 0 (since that's our origin)

			// line 2:  y - y_p = m2 * (x - x_p)
			//                m2 = -1.0 / m1

			// combining equations (upon intersection),
			//     y_s + m1*(x-x_s) = y_p + m2*(x-x_p)
			// solves down to:
			//       x = (m1*x_s - y_s + y_p - m2*s_p) / (m1 - m2)

			double m1 = end_vec.getY().getDoubleValue(Length::METERS)
					/ end_vec.getX().getDoubleValue(Length::METERS);
			double m2 = -1.0 / m1;

			double x = (spot_vec.getY().getDoubleValue(Length::METERS) -
					m2 * spot_vec.getX().getDoubleValue(Length::METERS)) / (m1 - m2);
			double y = m1 * x;

			Vector3D intersect(Length(x, Length::METERS), Length(y, Length::METERS), Length::ZERO);
			closestPoint = _start.move(intersect);
		}
	}

	// compute distance from path (make it negative if its left of path)
	error.DistanceError = closestPoint.getGroundRangeTo(currPos);
	NorthBearingAngle closestToPos = closestPoint.getBearingTo(currPos);
	Angle diff = closestToPos.minus(_path_bearing).correctFullCircle();
	if (diff > Angle::HALF_CIRCLE)
		error.DistanceError = error.DistanceError.scale(-1.0);

	error.BearingError = currHeading.minus(_path_bearing).correctHalfCircle();
	error.BearingAtPath = _path_bearing;
}



