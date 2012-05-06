#include "Length.h"

namespace cole {
namespace math {

const Length Length::ZERO(0.0, METERS);
const Length Length::ONE_METER(1.0, METERS);
const Length Length::AGIG_DISH_RANGE(120.0, NAUTICAL_MILES);
const Length Length::AGIG_OMNI_RANGE(3.0, NAUTICAL_MILES);

Length::Length() {
	_val_m = 0.0;
}
Length::Length(const Length& len) {
	_val_m = len._val_m;
}
Length::Length(double val, LengthUnit unit) {
	switch (unit) {
	case METERS:
	case UNITLESS:
		_val_m = val;
		break;
	case FEET:
		_val_m = val * Constants::FEET2METERS;
		break;
	case NAUTICAL_MILES:
		_val_m = val * Constants::NAUTICALMILES2METERS;
		break;
	default:
		throw invalid_argument("Unknown LengthUnit");
	}
}
Length::~Length() {
	;
}

double Length::getDoubleValue(LengthUnit unit) const {
	switch (unit) {
	case METERS:
	case UNITLESS:
		return _val_m;
	case FEET:
		return _val_m * Constants::METERS2FEET;
	case NAUTICAL_MILES:
		return _val_m * Constants::METERS2NAUTICALMILES;
	default:
		throw invalid_argument("Unknown LengthUnit");
	}
}
Length Length::scale(double scalar) const {
	return Length(_val_m * scalar, METERS);
}
Length Length::plus(const Length& other) const {
	return Length(_val_m + other._val_m, METERS);
}
Length Length::minus(const Length& other) const {
	return Length(_val_m - other._val_m, METERS);
}
Length Length::abs() const {
	return Length((_val_m >= 0.0 ? _val_m : -_val_m), METERS);
}
bool Length::operator==(const Length& other) const {
	return (_val_m == other._val_m);
}
bool Length::operator<=(const Length& other) const {
	return (_val_m <= other._val_m);
}
bool Length::operator<(const Length& other) const {
	return (_val_m < other._val_m);
}
bool Length::operator>(const Length& other) const {
	return (_val_m > other._val_m);
}
bool Length::operator>=(const Length& other) const {
	return (_val_m >= other._val_m);
}
Length& Length::operator=(const Length& other) {
	_val_m = other._val_m;
	return *this;
}
string Length::toString() {
	return toString(METERS);
}
string Length::toString(LengthUnit unit) {
	string ret("[Length: ");
	ostringstream sstream;

	switch (unit) {
	case METERS:
		sstream << _val_m;
		ret += sstream.str();
		ret += " m]";
		break;
	case FEET:
		sstream << _val_m * Constants::METERS2FEET;
		ret += sstream.str();
		ret += " ft]";
		break;
	case NAUTICAL_MILES:
		sstream << _val_m * Constants::METERS2NAUTICALMILES;
		ret += sstream.str();
		ret += " nm]";
		break;
	case UNITLESS:
		sstream << _val_m;
		ret += sstream.str();
		ret += "]";
		break;
	default:
		throw invalid_argument("Unknown LengthUnit");
	}
	return ret;
}

} // namespace math
} // namespace edu
