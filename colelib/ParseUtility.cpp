#include "ParseUtility.h"


namespace cole {
namespace util {

// parse exception
ParseException::ParseException(const string &message)
  throw() : userMessage(message) {
}
ParseException::~ParseException() throw() {
}
const char *ParseException::what() const throw() {
  return userMessage.c_str();
}


vector<unsigned char> ParseUtility::ConvertIpToBytes(const string &ip) {
	vector<unsigned char> bytes;
	string theip(ip);

	boost::algorithm::trim(theip);
	vector<string> toks;
	boost::algorithm::split(toks, theip, boost::algorithm::is_any_of("."));
	if (toks.size() == 4) {
		BOOST_FOREACH(string t, toks) {
			bytes.push_back( (unsigned char)atoi(t.c_str()) );
		}
	} else {
		throw ParseException("Unable to convert Ip string to bytes: "+ip);
	}
	return bytes;
}

int ParseUtility::FindMatchingIp(const vector<string> &compips, const string &ipToMatch) {
	int bestIndex=-1, bestNumMatches=-1;
	vector<string> toks;
	unsigned char ipBytes[4];

	boost::algorithm::split(toks, ipToMatch, boost::algorithm::is_any_of("."));
	if (toks.size() != 4)
		return -1;
	for (int i=0; i<4; i++)
		ipBytes[i] = (unsigned char)atoi(toks.at(i).c_str());

	for (int ip_idx=0; ip_idx<(int)compips.size(); ip_idx++) {
		string ip(compips.at(ip_idx));

		toks.clear();
		boost::algorithm::trim(ip);
		boost::algorithm::split(toks, ip, boost::algorithm::is_any_of("."));
		if (toks.size() != 4)
			continue;

		int numMatches = 0;
		for (int i=0; i<4; i++) {
			unsigned char uc = (unsigned char)atoi(toks.at(i).c_str());
			if (uc != ipBytes[i])
				break;
			if (++numMatches > bestNumMatches) {
				bestNumMatches = numMatches;
				bestIndex = ip_idx;
			}
		}
	}
	return bestIndex;
}

long ParseUtility::ConvertIpToLong(const string &ip) {
	long l = 0L;
	string theip(ip);

	boost::algorithm::trim(theip);
	vector<string> toks;
	boost::algorithm::split(toks, theip, boost::algorithm::is_any_of("."));
	if (toks.size() == 4) {
		for (int i=0; i<(int)toks.size(); i++) {
			unsigned char c = (unsigned char)atoi(toks.at(i).c_str());
			l |= ((c&0x00FF) << (24-(8*i)));
		}
	} else {
		throw ParseException("Unable to convert Ip string to long: "+ip);
	}
	return l;
}

bool ParseUtility::VerifyIP(const string &ip) {
	boost::system::error_code ec;
	boost::asio::ip::address::from_string( ip, ec );
	if ( ec )
		return false;
	return true;
}

bool ParseUtility::ParseBool(const string &str) {
	string s(str);
	boost::to_lower(s);
	boost::algorithm::trim(s);
	if (s == "true")
		return true;
	if (s == "false")
		return false;
	throw ParseException("Unable to convert string to bool: "+str);
}

int ParseUtility::ParseInt(const string &str) {
	string s(str);
	boost::algorithm::trim(s);

	if (s.length() == 0)
		throw ParseException("Empty String");
	for (int i=0; i<(int)s.length(); i++) {
		if (i==0 && s[i]=='-')
			continue;
		if (!isdigit(s[i]))
			throw ParseException("Unable to convert string to int: "+str);
	}
	return atoi(s.c_str());
}
unsigned short ParseUtility::ParseUShort(const string &str) {
	return (unsigned short)(ParseInt(str) & 0x00FFFF);
}
unsigned char ParseUtility::ParseUChar(const string &str) {
	return (unsigned char)(ParseInt(str) & 0x00FF);
}
double ParseUtility::ParseDouble(const string &str) {
	string s(str);
	boost::algorithm::trim(s);
	if (s.length() == 0)
		throw ParseException("Empty String");
	bool d=false;
	for (int i=0; i<(int)s.length(); i++) {
		if (i==0 && s[i]=='-')
			continue;
		if (s[i] == '.' && !d) {
			d = true;
			continue;
		}
		if (!isdigit(s[i]))
			throw ParseException("Unable to convert string to double: "+str);
	}
	return atof(s.c_str());
}
float ParseUtility::ParseFloat(const string &str) {
	return (float)ParseDouble(str);
}


} // namespace util
} // namespace cole
