#include "Vector3D.h"

namespace cole {
namespace math {
namespace position {

const Vector3D Vector3D::ZERO(Length::ZERO, Length::ZERO, Length::ZERO);

Vector3D::Vector3D() {
	_x = Length::ZERO;
	_y = Length::ZERO;
	_z = Length::ZERO;
}
Vector3D::Vector3D(const Vector3D &vec) {
	_x = vec.getX();
	_y = vec.getY();
	_z = vec.getZ();
}
Vector3D::Vector3D(Length x, Length y, Length z) {
	_x = x;
	_y = y;
	_z = z;
}
Vector3D::~Vector3D() {}

Length Vector3D::getX() const {
	return _x;
}
Length Vector3D::getY() const {
	return _y;
}
Length Vector3D::getZ() const {
	return _z;
}

bool Vector3D::operator==(const Vector3D& other) const {
	return (_x==other._x && _y==other._y && _z==other._z);
}
bool Vector3D::operator!=(const Vector3D& other) const {
	return !operator==(other);
}


string Vector3D::toString() {
	ostringstream oss;
	oss << _x.getDoubleValue(Length::METERS);
	oss <<  ",";
	oss << _y.getDoubleValue(Length::METERS);
	oss << ",";
	oss << _z.getDoubleValue(Length::METERS);
	return oss.str();
}

Length Vector3D::getRangeTo(const Vector3D &other) {
	Vector3D diff = getVectorTo(other);
	double x = diff.getX().getDoubleValue(Length::METERS);
	double y = diff.getY().getDoubleValue(Length::METERS);
	double z = diff.getZ().getDoubleValue(Length::METERS);
	double dist = sqrt( x*x + y*y + z*z );
	return Length(dist, Length::METERS);
}

Length Vector3D::getGroundRangeTo(const Vector3D &other) {
	Vector3D diff = getVectorTo(other);
	double x = diff.getX().getDoubleValue(Length::METERS);
	double y = diff.getY().getDoubleValue(Length::METERS);
	double dist = sqrt( x*x + y*y );
	return Length(dist, Length::METERS);
}

NorthBearingAngle Vector3D::getBearingTo(const Vector3D &other) {
	Vector3D diff = getVectorTo(other);
	if (diff == Vector3D::ZERO)
		return NorthBearingAngle::NORTH;
	double x = diff.getX().getDoubleValue(Length::METERS);
	double y = diff.getY().getDoubleValue(Length::METERS);
	Angle ang( atan2(y,x), Angle::RADIANS );
	return ang.toNorthBearing();
}

Vector3D Vector3D::getVectorTo(const Vector3D &other) {
	Length x = other.getX().minus(_x);
	Length y = other.getY().minus(_y);
	Length z = other.getZ().minus(_z);
	return Vector3D(x, y, z);
}

Vector3D Vector3D::move(const Vector3D &movement) {
	Length x = movement.getX().plus(_x);
	Length y = movement.getY().plus(_y);
	Length z = movement.getZ().plus(_z);
	return Vector3D(x, y, z);
}

Vector3D Vector3D::move(const NorthBearingAngle &bearing, const Length &dist) {
	double m = dist.getDoubleValue(Length::METERS);
	double rad = bearing.toAngle().getDoubleValue(Angle::RADIANS);
	double x_m = m * cos(rad);
	double y_m = m * sin(rad);
	Length x = _x.plus(Length(x_m, Length::METERS));
	Length y = _y.plus(Length(y_m, Length::METERS));
	return Vector3D(x, y, _z);
}

} // namespace position
} // namespace math
} // namespace cole
