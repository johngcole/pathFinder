#include "ArcPath.h"

ArcPath::ArcPath(Position3D center, Length radius, NorthBearingAngle start,
		NorthBearingAngle end, bool clockwise) {

	_center = center;
	_radius = radius;
	_arc_start = start;
	_arc_end = end;
	_clockwise = clockwise;
	_arc_len = (clockwise ? _arc_start.clockwiseAngleTo(_arc_end)
			: _arc_start.counterClockwiseAngleTo(_arc_end));
}

ArcPath::ArcPath(Length radius, Position3D startLocation,
		NorthBearingAngle startBearing, Length length, bool clockwise) {

	_clockwise = clockwise;
	_radius = radius;
	NorthBearingAngle toCenter = (_clockwise ? startBearing.plus(
			Angle::RIGHT_ANGLE) : startBearing.minus(Angle::RIGHT_ANGLE));
	_center = startLocation.move(toCenter, _radius);
	_arc_start = _center.getBearingTo(startLocation);

	double percent = 2 * Constants::PI * radius.getDoubleValue(Length::METERS);
	percent /= length.getDoubleValue(Length::METERS);
	_arc_len = Angle::FULL_CIRCLE.scale(1.0 / percent);
	_arc_end = (_clockwise ? _arc_start.plus(_arc_len) : _arc_start.minus(
			_arc_len));
}

Length ArcPath::length() {
	double circum = ((Constants::PI * 2.0) * _radius.getDoubleValue(
			Length::METERS));
	double arcPer = (_arc_len.getDoubleValue(Angle::RADIANS) / (Constants::PI
			* 2.0));
	return Length(circum * arcPer, Length::METERS);
}
Position3D ArcPath::start() {
	return _center.move(_arc_start, _radius);
}
Position3D ArcPath::end() {
	return _center.move(_arc_end, _radius);
}
NorthBearingAngle ArcPath::startBearing() {
	return (_clockwise ? _arc_start.plus(Angle::RIGHT_ANGLE)
			: _arc_start.minus(Angle::RIGHT_ANGLE));
}
NorthBearingAngle ArcPath::endBearing() {
	return (_clockwise ? _arc_end.plus(Angle::RIGHT_ANGLE) : _arc_end.minus(
			Angle::RIGHT_ANGLE));
}

bool ArcPath::isBearingWithinArcPath(NorthBearingAngle bearing) {
	bearing = bearing.correctFullCircle();
	if (_clockwise) {
		return (bearing >= _arc_start) && (bearing <= _arc_end);
	} else {
		return (bearing <= _arc_start) && (bearing >= _arc_end);
	}
}

Position3D ArcPath::findPosition(float percentage) {
	if (percentage <= 0.0)
		return start();
	if (percentage >= 1.0)
		return end();

	Angle totTravel = (_clockwise ? _arc_start.clockwiseAngleTo(_arc_end)
			: _arc_start.counterClockwiseAngleTo(_arc_end));
	Angle perTravel = totTravel.scale(percentage);
	NorthBearingAngle fromCenter = (_clockwise ? _arc_start.plus(perTravel)
			: _arc_start.minus(perTravel));

	return _center.move(fromCenter, _radius);
}

Position3D ArcPath::findPosition(Length dist) {
	double arclen = length().getDoubleValue(Length::METERS);
	double dlen = dist.getDoubleValue(Length::METERS);
	float per = dlen / arclen;
	return findPosition(per);
}

void ArcPath::fillPathError(Position3D currPos, NorthBearingAngle currHeading,
		PathError &err) {

	// first, we need to find the closest point on the arc to this given currPos
	Position3D closestPoint;
	NorthBearingAngle toPos = _center.getBearingTo(currPos);
	//System.out.println("Angle from center to point -> "+toPos.toString(Angle.DEGREES));
	if (isBearingWithinArcPath(toPos)) {
		// currPos bearing is within our arc, so we just pick that bearing's point on arc
		closestPoint = _center.move(toPos, _radius);
	} else {
		// currPos bearing is outside our arc, so we either pick end or start as closest point
		Length toStart = start().getRangeTo(currPos);
		Length toEnd = end().getRangeTo(currPos);
		if (toStart <= toEnd)
			closestPoint = start();
		else
			closestPoint = end();
	}
	err.DistanceError = closestPoint.getRangeTo(currPos);

	NorthBearingAngle toClosest = _center.getBearingTo(closestPoint);
	err.BearingAtPath = (_clockwise ? toClosest.plus(Angle::RIGHT_ANGLE)
			: toClosest.minus(Angle::RIGHT_ANGLE));
	err.BearingError = currHeading.minus(err.BearingAtPath).correctHalfCircle();

	// if currPos is left of path, we need to negate distance error
	NorthBearingAngle closestToPos = closestPoint.getBearingTo(currPos);
	Angle pathBearingDiff = err.BearingAtPath.clockwiseAngleTo(closestToPos);
	if (pathBearingDiff > Angle::HALF_CIRCLE)
		err.DistanceError = err.DistanceError.scale(-1.0);
}

