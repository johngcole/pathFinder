#include "Velocity.h"

namespace cole {
namespace math {

const Velocity Velocity::ZERO(0.0, MPS);

Velocity::Velocity() {
	_val_mps = 0.0;
}
Velocity::Velocity(const Velocity& len) {
	_val_mps = len._val_mps;
}
Velocity::Velocity(double val, VelocityUnit unit) {
	switch (unit) {
	case MPS:
	case UNITLESS:
		_val_mps = val;
		break;
	case KNOTS:
		_val_mps = val * Constants::KNOTS2MPS;
		break;
	default:
		throw invalid_argument("Unknown VelocityUnit");
	}
}
Velocity::~Velocity() {
	;
}

double Velocity::getDoubleValue(VelocityUnit unit) const {
	switch (unit) {
	case MPS:
	case UNITLESS:
		return _val_mps;
	case KNOTS:
		return _val_mps * Constants::MPS2KNOTS;
	default:
		throw invalid_argument("Unknown VelocityUnit");
	}
}
Velocity Velocity::scale(double scalar) const {
	return Velocity(_val_mps * scalar, MPS);
}
Velocity Velocity::plus(const Velocity& other) const {
	return Velocity(_val_mps + other._val_mps, MPS);
}
Velocity Velocity::minus(const Velocity& other) const {
	return Velocity(_val_mps - other._val_mps, MPS);
}
bool Velocity::operator==(const Velocity& other) const {
	return (_val_mps == other._val_mps);
}
bool Velocity::operator<=(const Velocity& other) const {
	return (_val_mps <= other._val_mps);
}
bool Velocity::operator<(const Velocity& other) const {
	return (_val_mps < other._val_mps);
}
bool Velocity::operator>(const Velocity& other) const {
	return (_val_mps > other._val_mps);
}
bool Velocity::operator>=(const Velocity& other) const {
	return (_val_mps >= other._val_mps);
}
Velocity& Velocity::operator=(const Velocity& other) {
	_val_mps = other._val_mps;
	return *this;
}
string Velocity::toString() {
	return toString(MPS);
}
string Velocity::toString(VelocityUnit unit) {
	string ret("[Velocity: ");
	ostringstream sstream;

	switch (unit) {
	case MPS:
		sstream << _val_mps;
		ret += sstream.str();
		ret += " mps]";
		break;
	case KNOTS:
		sstream << _val_mps * Constants::MPS2KNOTS;
		ret += sstream.str();
		ret += " kts]";
		break;
	case UNITLESS:
		sstream << _val_mps;
		ret += sstream.str();
		ret += "]";
		break;
	default:
		throw invalid_argument("Unknown VelocityUnit");
	}
	return ret;
}

} // namespace math
} // namespace cole
