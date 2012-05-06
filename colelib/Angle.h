// Angle.h
#ifndef __ANGLE_H__
#define __ANGLE_H__

#include <string>
#include <sstream>
#include "Constants.h"

using namespace std;
using namespace cole::util;

namespace cole {
namespace math {

class NorthBearingAngle;

class Angle {
public:
	enum AngleUnit {
		RADIANS, DEGREES, UNITLESS
	};

	static Angle const ZERO;
	static Angle const RIGHT_ANGLE;
	static Angle const HALF_CIRCLE;
	static Angle const FULL_CIRCLE;
	static Angle const AGIG_DISH_BEAMWIDTH;

	static Angle const INVALID_ANGLE;

	Angle();
	Angle(double val, AngleUnit unit);
	Angle(const Angle& ang);
	~Angle();

	double getDoubleValue(AngleUnit unit) const;
	Angle scale(double scalar) const;
	Angle plus(const Angle& other) const;
	Angle minus(const Angle& other) const;
	Angle abs() const;
	Angle correctFullCircle() const;
	Angle correctHalfCircle() const;

	NorthBearingAngle toNorthBearing() const;

	Angle& operator=(const Angle& other);

	bool operator==(const Angle& other) const;
	bool operator!=(const Angle& other) const;
	bool operator<=(const Angle& other) const;
	bool operator<(const Angle& other) const;
	bool operator>(const Angle& other) const;
	bool operator>=(const Angle& other) const;
	string toString();
	string toString(AngleUnit unit);

protected:
	double _val_rad;
};

} // namespace math
} // namespace cole

#endif
