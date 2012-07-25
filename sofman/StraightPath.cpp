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

void StraightPath::fillPathError(Position3D currPos,
		NorthBearingAngle currHeading, PathError &error) {

	Length r_to_start = _start.getGroundRangeTo(currPos);
	Length r_to_end = _end.getGroundRangeTo(currPos);
	Position3D middle = _start.move(_path_bearing, length().scale(0.5));
	Length r_to_mid = middle.getGroundRangeTo(currPos);

	Position3D closestPoint;
	if ((r_to_mid <= r_to_start && r_to_mid <= r_to_end) ||
			(r_to_mid >= r_to_start && r_to_mid >= r_to_end)) {
		// the point is outside the perpindicular plane of the straight path
		//     closest point is just closer between start and end
		closestPoint = (r_to_end <= r_to_start) ? _end : _start;
	} else {
		// ok, now we can do our intesect

		Vector3D end_vec = _start.getVectorTo(_end);
		Vector3D spot_vec = _start.getVectorTo(currPos);

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

	// compute distance from path (make it negative if its left of path)
	error.DistanceError = closestPoint.getRangeTo(currPos);
	NorthBearingAngle closestToPos = closestPoint.getBearingTo(currPos);
	Angle diff = closestToPos.minus(_path_bearing).correctFullCircle();
	if (diff > Angle::HALF_CIRCLE)
		error.DistanceError = error.DistanceError.scale(-1.0);

	error.BearingError = currHeading.minus(_path_bearing).correctHalfCircle();
	error.BearingAtPath = _path_bearing;
}



