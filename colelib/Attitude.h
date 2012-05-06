// Attitude.h
#ifndef __ATTITUDE_H__
#define __ATTITUDE_H__

#include <string>
#include <cmath>
#include "Constants.h"
#include "Angle.h"
#include "NorthBearingAngle.h"

using namespace std;
using namespace cole::util;

namespace cole {
namespace math {

class Attitude {
public:
	static Attitude const INVALID_ATT;

	Attitude();
	Attitude(const Attitude &att);
	Attitude(const Angle &roll, const Angle &pitch,
			const NorthBearingAngle &yaw);
	~Attitude();

	Angle getRoll() const;
	Angle getPitch() const;
	NorthBearingAngle getYaw() const;

	bool operator==(const Attitude& other) const;
	bool operator!=(const Attitude& other) const;

	string toString();

protected:
	Angle _roll, _pitch;
	NorthBearingAngle _yaw;
};

} // namespace math
} // namespace cole

#endif
