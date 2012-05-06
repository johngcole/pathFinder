#include "Angle.h"
#include "NorthBearingAngle.h"

namespace cole {
namespace math {

const Angle Angle::ZERO(0.0, DEGREES);
const Angle Angle::RIGHT_ANGLE(90.0, DEGREES);
const Angle Angle::HALF_CIRCLE(180.0, DEGREES);
const Angle Angle::FULL_CIRCLE(360.0, DEGREES);
const Angle Angle::AGIG_DISH_BEAMWIDTH(5.0, DEGREES);

const Angle Angle::INVALID_ANGLE(-999.0, DEGREES);

Angle::Angle() {
	_val_rad = 0.0;
}
Angle::Angle(const Angle& ang) {
	_val_rad = ang._val_rad;
}
Angle::Angle(double val, AngleUnit unit) {
	switch (unit) {
	case RADIANS:
	case UNITLESS:
		_val_rad = val;
		break;
	case DEGREES:
		_val_rad = val * Constants::DEG2RAD;
		break;
	default:
		throw invalid_argument("Unknown AngleUnit");
	}
}
Angle::~Angle() {
	;
}

double Angle::getDoubleValue(AngleUnit unit) const {
	switch (unit) {
	case RADIANS:
	case UNITLESS:
		return _val_rad;
	case DEGREES:
		return _val_rad * Constants::RAD2DEG;
	default:
		throw invalid_argument("Unknown AngleUnit");
	}
}
Angle Angle::scale(double scalar) const {
	return Angle(_val_rad * scalar, RADIANS);
}
Angle Angle::plus(const Angle& other) const {
	return Angle(_val_rad + other._val_rad, RADIANS);
}
Angle Angle::minus(const Angle& other) const {
	return Angle(_val_rad - other._val_rad, RADIANS);
}
Angle Angle::correctFullCircle() const {
	Angle ang(_val_rad, Angle::RADIANS);
	while (ang >= Angle::FULL_CIRCLE)
		ang = ang.minus(Angle::FULL_CIRCLE);
	while (ang < Angle::ZERO)
		ang = ang.plus(Angle::FULL_CIRCLE);
	return ang;
}
Angle Angle::correctHalfCircle() const {
	Angle ang(_val_rad, Angle::RADIANS);
	while (ang >= Angle::HALF_CIRCLE)
		ang = ang.minus(Angle::FULL_CIRCLE);
	while (ang < Angle::HALF_CIRCLE.scale(-1.0))
		ang = ang.plus(Angle::FULL_CIRCLE);
	return ang;
}
Angle Angle::abs() const {
	Angle ang(_val_rad, Angle::RADIANS);
	if (ang < Angle::ZERO)
		return ang.scale(-1.0);
	return ang;
}

NorthBearingAngle Angle::toNorthBearing() const {
	NorthBearingAngle nba(-_val_rad, Angle::RADIANS);
	return nba.minus(NorthBearingAngle::EAST);
}

bool Angle::operator==(const Angle& other) const {
	return (_val_rad == other._val_rad);
}
bool Angle::operator!=(const Angle& other) const {
	return !operator==(other);
}
bool Angle::operator<=(const Angle& other) const {
	return (_val_rad <= other._val_rad);
}
bool Angle::operator<(const Angle& other) const {
	return (_val_rad < other._val_rad);
}
bool Angle::operator>(const Angle& other) const {
	return (_val_rad > other._val_rad);
}
bool Angle::operator>=(const Angle& other) const {
	return (_val_rad >= other._val_rad);
}
Angle& Angle::operator=(const Angle& other) {
	_val_rad = other._val_rad;
	return *this;
}
string Angle::toString() {
	return toString(DEGREES);
}
string Angle::toString(AngleUnit unit) {
	string ret("[Angle: ");
	ostringstream sstream;

	switch (unit) {
	case RADIANS:
		sstream << _val_rad;
		ret += sstream.str();
		ret += " rad]";
		break;
	case DEGREES:
		sstream << _val_rad * Constants::RAD2DEG;
		ret += sstream.str();
		ret += " deg]";
		break;
	case UNITLESS:
		sstream << _val_rad;
		ret += sstream.str();
		ret += "]";
		break;
	default:
		throw invalid_argument("Unknown AngleUnit");
	}
	return ret;
}

} // namespace math
} // namespace cole
