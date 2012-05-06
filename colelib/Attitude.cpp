#include "Attitude.h"

namespace cole {
namespace math {

const Attitude Attitude::INVALID_ATT(Angle(-999.0, Angle::DEGREES), Angle(
		-999.0, Angle::DEGREES), NorthBearingAngle(-999.0, Angle::DEGREES));

Attitude::Attitude() :
	_roll(), _pitch(), _yaw() {
	;
}
Attitude::Attitude(const Angle &roll, const Angle &pitch,
		const NorthBearingAngle &yaw) {
	_roll = roll;
	_pitch = pitch;
	_yaw = yaw;
}
Attitude::Attitude(const Attitude &att) {
	_roll = att._roll;
	_pitch = att._pitch;
	_yaw = att._yaw;
}
Attitude::~Attitude() {
	;
}

Angle Attitude::getRoll() const {
	return _roll;
}
Angle Attitude::getPitch() const {
	return _pitch;
}
NorthBearingAngle Attitude::getYaw() const {
	return _yaw;
}

string Attitude::toString() {
	string ret("Attitude: <");
	ret += _roll.toString() + " ";
	ret += _pitch.toString() + " ";
	ret += _yaw.toString() + ">";
	return ret;
}

bool Attitude::operator==(const Attitude& other) const {
	bool ans = (_roll == other._roll);
	ans = ans && (_pitch == other._pitch);
	ans = ans && (_yaw == other._yaw);
	return ans;
}

bool Attitude::operator!=(const Attitude& other) const {
	return !operator==(other);
}

} // namespace math
} // namespace cole
