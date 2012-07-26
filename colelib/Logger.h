#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <boost/thread/mutex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

namespace cole {
namespace util {

class Logger {

public:
	static void setLoggerFile(const string logFilename);
	static void setLoggerFile(const char* logFilename);
	static Logger* getInstance();
	void log(const string msg);
	void log(const char* msg);

private:
	static Logger* _logger;
	static string _logFilename;
	static bool _logFileSet;
	boost::mutex _logMutex;
	Logger();
	~Logger();
};

}
}

#endif

