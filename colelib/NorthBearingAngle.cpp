#include "NorthBearingAngle.h"

namespace cole {
namespace math {

const NorthBearingAngle NorthBearingAngle::NORTH(0.0, DEGREES);
const NorthBearingAngle NorthBearingAngle::EAST(90.0, DEGREES);
const NorthBearingAngle NorthBearingAngle::SOUTH(180.0, DEGREES);
const NorthBearingAngle NorthBearingAngle::WEST(270.0, DEGREES);

const NorthBearingAngle NorthBearingAngle::SOUTHWEST(225.0, DEGREES);
const NorthBearingAngle NorthBearingAngle::NORTHWEST(315.0, DEGREES);
const NorthBearingAngle NorthBearingAngle::SOUTHEAST(135.0, DEGREES);
const NorthBearingAngle NorthBearingAngle::NORTHEAST(45.0, DEGREES);

NorthBearingAngle::NorthBearingAngle() :
	Angle() {
	;
}
NorthBearingAngle::NorthBearingAngle(const NorthBearingAngle& ang) {
	_val_rad = ang._val_rad;
}
NorthBearingAngle::NorthBearingAngle(const Angle& ang) {
	_val_rad = ang.getDoubleValue(Angle::RADIANS);
}
NorthBearingAngle::NorthBearingAngle(double val, AngleUnit unit) :
	Angle(val, unit) {
	;
}
NorthBearingAngle::~NorthBearingAngle() {
	;
}

Angle NorthBearingAngle::toAngle() const {
	NorthBearingAngle nba = correctFullCircle();
	Angle ang(-nba.getDoubleValue(Angle::RADIANS), Angle::RADIANS);
	return ang.minus(Angle::RIGHT_ANGLE);
}

NorthBearingAngle& NorthBearingAngle::operator=(const Angle& other) {
	_val_rad = other.getDoubleValue(Angle::RADIANS);
	return *this;
}

string NorthBearingAngle::toString() {
	return toString(Angle::DEGREES);
}
string NorthBearingAngle::toString(AngleUnit unit) {
	string ret("[NorthBearingAngle: ");
	ostringstream sstream;

	switch (unit) {
	case RADIANS:
		sstream << _val_rad;
		ret += sstream.str();
		ret += " rad T]";
		break;
	case DEGREES:
		sstream << _val_rad * Constants::RAD2DEG;
		ret += sstream.str();
		ret += " deg T]";
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
