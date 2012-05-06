#include "Config.h"

namespace cole {
namespace util {

// config exception
ConfigException::ConfigException(const string &message) throw () :
	userMessage(message) {
}
ConfigException::~ConfigException() throw () {
}
const char *ConfigException::what() const throw () {
	return userMessage.c_str();
}

Config::Config(string filename) :
	_configFilename(filename), _configMutex(), _configVars() {
	_hold = false;

	// now read in what we got
	if (!this->readInConfigValues())
		throw ConfigException("CONFIG: Unable to parse input file: " + filename);
}

Config::~Config() {
	;
}

void Config::setHold(bool hold) {
	_configMutex.lock();
	_hold = hold;
	if (!_hold)
		writeOutToFile();
	_configMutex.unlock();
}

///////////////////////////////////////////////////////////////////////////////////////////////
//                                  Get/Set for config values
////////////////////////////////////////////////////////////////////////////////////////////////
string Config::getValue(const string name) {
	string val;
	bool found = false;

	_configMutex.lock();
	map<string, string>::iterator it = _configVars.find(name);
	if (it != _configVars.end()) {
		found = true;
		val = it->second;
	}
	_configMutex.unlock();

	if (!found)
		throw ConfigException("CONFIG: Unable to get value of: " + name);
	return val;
}
int Config::getValueAsInt(const string name) {
	string value;

	try {
		value = getValue(name);
	} catch (ConfigException ce) {
		throw ce;
	}

	return atoi(value.c_str());
}
double Config::getValueAsDouble(const string name) {
	string value;

	try {
		value = getValue(name);
	} catch (ConfigException ce) {
		throw ce;
	}

	return atof(value.c_str());
}
bool Config::getValueAsBool(const string name) {
	string value;

	try {
		value = getValue(name);
	} catch (ConfigException ce) {
		throw ce;
	}

	boost::to_lower(value);
	if (value.find("true") != string::npos)
		return true;
	return false;
}

//----------------------------------------------------------------------------------


void Config::setValue(const string name, string value) {
	bool found = false;

	_configMutex.lock();
	map<string, string>::iterator it = _configVars.find(name);
	if (it != _configVars.end()) {
		found = true;
		_configVars[name] = value;
	}
	if (!_hold && found)
		writeOutToFile();
	_configMutex.unlock();

	if (!found)
		throw ConfigException("CONFIG: Unable to set value of: " + name);
}
void Config::setValueAsInt(const string name, int value) {
	ostringstream sstream;
	sstream << value;

	try {
		setValue(name, sstream.str());
	} catch (ConfigException e) {
		throw e;
	}
}
void Config::setValueAsDouble(const string name, double value) {
	ostringstream sstream;
	sstream << value;

	try {
		setValue(name, sstream.str());
	} catch (ConfigException e) {
		throw e;
	}
}
void Config::setValueAsBool(const string name, bool value) {
	try {
		setValue(name, (value ? "true" : "false"));
	} catch (ConfigException e) {
		throw e;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
//                         Creating / Deleting config values

void Config::deleteValue(const string name) {
	bool found = false;

	_configMutex.lock();
	map<string, string>::iterator it = _configVars.find(name);
	if (it != _configVars.end()) {
		found = true;
		_configVars.erase(it);
		writeOutToFile();
	}
	_configMutex.unlock();

	if (!found)
		throw ConfigException("CONFIG: Unable to delete value of: " + name);
}

void Config::createValue(const string name, string value) {
	bool found = false;

	_configMutex.lock();
	map<string, string>::iterator it = _configVars.find(name);
	if (it != _configVars.end()) {
		found = true;
	} else {
		string var_name(name);
		boost::algorithm::trim(var_name);
		string var_value(value);
		boost::algorithm::trim(var_value);
		_configVars.insert(pair<string, string> (var_name, var_value));
		writeOutToFile();
	}
	_configMutex.unlock();

	if (found)
		throw ConfigException("CONFIG: Tried to create already existing name: "
				+ name);
}

void Config::createValueAsInt(const string name, int value) {
	ostringstream sstream;
	sstream << value;
	try {
		createValue(name, sstream.str());
	} catch (ConfigException e) {
		throw e;
	}
}
void Config::createValueAsDouble(const string name, double value) {
	ostringstream sstream;
	sstream << value;
	try {
		createValue(name, sstream.str());
	} catch (ConfigException e) {
		throw e;
	}
}
void Config::createValueAsBool(const string name, bool value) {
	try {
		createValue(name, (value ? "true" : "false"));
	} catch (ConfigException e) {
		throw e;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
//                                Reading in (parsing) config file
////////////////////////////////////////////////////////////////////////////////////////////////
bool Config::readInConfigValues() {
	string line;
	ifstream infile(_configFilename.c_str());

	if (!infile.is_open())
		return false;

	while (infile.good()) {

		getline(infile, line);
		boost::algorithm::trim(line);

		if (line.find("#") == 0)
			continue; // ignore lines starting with '#'

		vector<string> toks;
		boost::algorithm::split(toks, line, boost::algorithm::is_any_of("="));

		if (toks.size() != 2) {
			//cout << "CONFIG ERROR: Unreadable line: " << line << endl;
			continue;
		}

		string var_name(toks[0]);
		boost::algorithm::trim(var_name);
		string var_value(toks[1]);
		boost::algorithm::trim(var_value);

		_configVars.insert(pair<string, string> (var_name, var_value));
	}
	infile.close();
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////
//                                Printing out (saving) config file
////////////////////////////////////////////////////////////////////////////////////////////////
void Config::writeOutToFile() {
	ofstream outfile(_configFilename.c_str());

	if (!outfile.is_open()) {
		cout << "Config Error: unable to open Config file for writing: "
				<< _configFilename << endl;
		return;
	}

	pair<string, string> entry;
	BOOST_FOREACH( entry, _configVars )
{	outfile << entry.first << " = " << entry.second << endl;
}
outfile.close();
}
////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////
void Config::printDebug() {
cout << endl << "DEBUG CONFIG (map size = " << _configVars.size() << ")" << endl;
pair<string,string> entry;
BOOST_FOREACH( entry, _configVars ) {
	cout << "\t" << entry.first << " -> " << entry.second << endl;
}
cout << "--------------------------------------------" << endl << endl;
}

}
}

