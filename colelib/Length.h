// Length.h
#ifndef __LENGTH_H__
#define __LENGTH_H__

#include <string>
#include <sstream>
#include "Constants.h"

using namespace std;
using namespace cole::util;

namespace cole {
namespace math {

class Length {
public:
	enum LengthUnit {
		METERS, FEET, NAUTICAL_MILES, UNITLESS
	};

	static Length const ZERO;
	static Length const ONE_METER;
	static Length const AGIG_DISH_RANGE;
	static Length const AGIG_OMNI_RANGE;

	Length();
	Length(double val, LengthUnit unit);
	Length(const Length& len);
	~Length();

	double getDoubleValue(LengthUnit unit) const;
	Length scale(double scalar) const;
	Length plus(const Length& other) const;
	Length minus(const Length& other) const;
	Length abs() const;
	Length& operator=(const Length& other);
	bool operator==(const Length& other) const;
	bool operator<=(const Length& other) const;
	bool operator<(const Length& other) const;
	bool operator>(const Length& other) const;
	bool operator>=(const Length& other) const;
	string toString();
	string toString(LengthUnit unit);

protected:
	double _val_m;
};

} // namespace math
} // namespace cole

#endif
