// Vector3D.h
#ifndef __VECTOR_3D_H__
#define __VECTOR_3D_H__

#include <string>
#include <sstream>
#include <cmath>
#include "Constants.h"
#include "Logger.h"
#include "Length.h"
#include "NorthBearingAngle.h"

using namespace std;
using namespace cole::util;
using namespace cole::math;

namespace cole {
namespace math {
namespace position {

class Vector3D {
public:
	static Vector3D const ZERO;


	Vector3D();
	Vector3D(const Vector3D &vec);
	Vector3D(Length x, Length y, Length z);
	~Vector3D();

	Length getX() const;
	Length getY() const;
	Length getZ() const;
	bool operator==(const Vector3D& other) const;
	bool operator!=(const Vector3D& other) const;

	Length getRangeTo(const Vector3D &other);
	Length getGroundRangeTo(const Vector3D &other);
	NorthBearingAngle getBearingTo(const Vector3D &other);
	Vector3D getVectorTo(const Vector3D &other);
	Vector3D move(const Vector3D &movement);
	Vector3D move(const NorthBearingAngle &bearing, const Length &dist);

	string toString();

protected:
	Length _x, _y, _z;
};

} // namespace position
} // namespace math
} // namespace cole


#endif
