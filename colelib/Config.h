#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <boost/foreach.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/case_conv.hpp>

using namespace std;

namespace cole {
namespace util {

class ConfigException: public exception {
public:
	ConfigException(const string &message) throw ();
	~ConfigException() throw ();
	const char *what() const throw ();
private:
	string userMessage; // Exception message
};

class Config {

public:
	Config(string filename);
	~Config();

	string getValue(const string name);
	int getValueAsInt(const string name);
	double getValueAsDouble(const string name);
	bool getValueAsBool(const string name);

	void setValue(const string name, string value);
	void setValueAsInt(const string name, int value);
	void setValueAsDouble(const string name, double value);
	void setValueAsBool(const string name, bool value);

	void deleteValue(const string name);
	void createValue(const string name, string value);
	void createValueAsInt(const string name, int value);
	void createValueAsDouble(const string name, double value);
	void createValueAsBool(const string name, bool value);

	void setHold(bool hold);

	void printDebug();
private:
	void writeOutToFile();
	bool readInConfigValues();

	string _configFilename;
	boost::mutex _configMutex;
	map<string, string> _configVars;
	bool _hold;
};

}
}

#endif /* CONFIG_H */
