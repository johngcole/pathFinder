// Velocity3D.h
#ifndef __VELOCITY_3D_H__
#define __VELOCITY_3D_H__

#include <string>
#include <cmath>
#include "Constants.h"
#include "Velocity.h"

using namespace std;
using namespace cole::util;

namespace cole {
namespace math {

class Velocity3D {
public:
	static Velocity3D const INVALID_V3D;

	Velocity3D();
	Velocity3D(const Velocity3D &v3d);
	Velocity3D(const Velocity &vnorth, const Velocity &veast,
			const Velocity &vdown);
	~Velocity3D();

	Velocity getVNorth() const;
	Velocity getVEast() const;
	Velocity getVDown() const;

	bool operator==(const Velocity3D& other) const;
	bool operator!=(const Velocity3D& other) const;

	string toString();

protected:
	Velocity _vnorth, _veast, _vdown;
};

} // namespace math
} // namespace cole

#endif
