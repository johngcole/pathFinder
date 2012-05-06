#include "Constants.h"

namespace cole {
namespace util {

#define __PI__ 3.141592653589793238462643383279502884197

const double Constants::PI = __PI__;
const double Constants::RAD2DEG = (180.0 / __PI__);
const double Constants::DEG2RAD = (__PI__ / 180.0);
const double Constants::FEET2METERS = 0.3048;
const double Constants::METERS2FEET = 3.2808399;
const double Constants::NAUTICALMILES2METERS = 1852.0;
const double Constants::METERS2NAUTICALMILES = 0.0005399568;
const double Constants::MPS2KNOTS = 1.943844;
const double Constants::KNOTS2MPS = 0.5144444;

const double Constants::WGS84_Eccentricity = 0.0818191908426;
const double Constants::WGS84_SemiMajorAxisRadius_m = 6378137.0;

int Constants::round(double val) {
	double d;
	int i;

	if (val >= 0) {
		i = (int) val;
		d = val - (double) i;
		if (d < 0.5)
			return i;
		else
			return (i + 1);
	} else {
		i = (int) val;
		d = val - (double) i;
		if (d < -0.5)
			return (i - 1);
		else
			return i;
	}
}

string Constants::parsePreprocessorTimestamp(string __timestamp__) {
	vector<string> tokens;
	boost::split(tokens, __timestamp__, boost::is_any_of(" "));
	string dstr("");
	if (tokens[1] == "Jan")
		dstr += "1";
	if (tokens[1] == "Feb")
		dstr += "2";
	if (tokens[1] == "Mar")
		dstr += "3";
	if (tokens[1] == "Apr")
		dstr += "4";
	if (tokens[1] == "May")
		dstr += "5";
	if (tokens[1] == "Jun")
		dstr += "6";
	if (tokens[1] == "Jul")
		dstr += "7";
	if (tokens[1] == "Aug")
		dstr += "8";
	if (tokens[1] == "Sep")
		dstr += "9";
	if (tokens[1] == "Oct")
		dstr += "10";
	if (tokens[1] == "Nov")
		dstr += "11";
	if (tokens[1] == "Dec")
		dstr += "12";

	if (tokens[2].length() == 0)
		dstr += "/" + tokens[3] + "/" + tokens[5];
	else
		dstr += "/" + tokens[2] + "/" + tokens[4];

	return dstr;
}

int Constants::hashString(string s, int numSlots) {
	unsigned long hash = 5381;

	for (unsigned int i = 0; i < s.length(); i++)
		hash = ((hash << 5) + hash) + s[i]; /* hash * 33 + c */

	return hash;
}

} // namespace util
} // namespace cole
