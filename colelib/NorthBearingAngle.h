// NorthBearingAngle.h
#ifndef __NORTH_BEARING_ANGLE_H__
#define __NORTH_BEARING_ANGLE_H__

#include <string>
#include <sstream>
#include "Constants.h"
#include "Angle.h"

using namespace std;
using namespace cole::util;

namespace cole {
namespace math {

class NorthBearingAngle: public Angle {
public:
	static NorthBearingAngle const NORTH;
	static NorthBearingAngle const SOUTH;
	static NorthBearingAngle const EAST;
	static NorthBearingAngle const WEST;

	static NorthBearingAngle const NORTHEAST;
	static NorthBearingAngle const NORTHWEST;
	static NorthBearingAngle const SOUTHEAST;
	static NorthBearingAngle const SOUTHWEST;

	NorthBearingAngle();
	NorthBearingAngle(double val, AngleUnit unit);
	NorthBearingAngle(const NorthBearingAngle& ang);
	NorthBearingAngle(const Angle& alt);
	~NorthBearingAngle();

	Angle toAngle() const;

	NorthBearingAngle& operator=(const Angle& other);

	string toString();
	string toString(AngleUnit unit);

protected:
};

} // namespace math
} // namespace cole

#endif
