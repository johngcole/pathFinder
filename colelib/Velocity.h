// Velocity.h
#ifndef __VELOCITY_H__
#define __VELOCITY_H__

#include <string>
#include <sstream>
#include "Constants.h"

using namespace std;
using namespace cole::util;

namespace cole {
namespace math {

class Velocity {
public:
	enum VelocityUnit {
		MPS, KNOTS, UNITLESS
	};

	static Velocity const ZERO;

	Velocity();
	Velocity(double val, VelocityUnit unit);
	Velocity(const Velocity& len);
	~Velocity();

	double getDoubleValue(VelocityUnit unit) const;
	Velocity scale(double scalar) const;
	Velocity plus(const Velocity& other) const;
	Velocity minus(const Velocity& other) const;
	Velocity& operator=(const Velocity& other);
	bool operator==(const Velocity& other) const;
	bool operator<=(const Velocity& other) const;
	bool operator<(const Velocity& other) const;
	bool operator>(const Velocity& other) const;
	bool operator>=(const Velocity& other) const;
	string toString();
	string toString(VelocityUnit unit);

protected:
	double _val_mps;
};

} // namespace math
} // namespace cole
#endif
