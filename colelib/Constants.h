// Constants.h
#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#include <boost/algorithm/string.hpp>
#include <vector>
#include <string>

using namespace std;

namespace cole {
namespace util {

#ifndef MIN
#define MIN(x,y)      (x<y ? x : y)
#endif
#ifndef MAX
#define MAX(x,y)      (x>y ? x : y)
#endif

class Constants {
public:
	static double const PI;
	static double const RAD2DEG;
	static double const DEG2RAD;
	static double const FEET2METERS;
	static double const METERS2FEET;
	static double const NAUTICALMILES2METERS;
	static double const METERS2NAUTICALMILES;
	static double const MPS2KNOTS;
	static double const KNOTS2MPS;

	static double const WGS84_Eccentricity;
	static double const WGS84_SemiMajorAxisRadius_m;

	static int round(double val);

	static string parsePreprocessorTimestamp(string __timestamp__);

	static int hashString(string s, int numSlots);
};

} // namespace math
} // namespace cole

#endif
