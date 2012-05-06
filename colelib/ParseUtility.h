// Constants.h
#ifndef __IP_UTILITY_H__
#define __IP_UTILITY_H__

#include <boost/system/error_code.hpp>
#include <boost/asio/ip/address.hpp>

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>

#include <algorithm>
#include <string> 
#include <vector>

using namespace std;

namespace cole {
namespace util {

class ParseException: public exception {
public:
	ParseException(const string &message) throw ();
	~ParseException() throw ();
	const char *what() const throw ();
private:
	string userMessage; // Exception message
};

class ParseUtility {
public:
	// Ip Parsing
	static vector<unsigned char> ConvertIpToBytes(const string &ip);
	static int FindMatchingIp(const vector<string> &compips,
			const string &ipToMatch);
	static long ConvertIpToLong(const string &ip);
	static bool VerifyIP(const string &ip);

	// other parsing
	static bool ParseBool(const string &str);
	static int ParseInt(const string &str);
	static unsigned short ParseUShort(const string &str);
	static unsigned char ParseUChar(const string &str);
	static double ParseDouble(const string &str);
	static float ParseFloat(const string &str);

};

} // namespace math
} // namespace cole

#endif
