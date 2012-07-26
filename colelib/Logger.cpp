#include "Logger.h"

namespace cole {
namespace util {

Logger* Logger::_logger = NULL;
string Logger::_logFilename = string("");
bool Logger::_logFileSet = false;

void Logger::setLoggerFile(const string logFilename) {
	Logger::_logFilename = logFilename;
	Logger::_logFileSet = true;
}
void Logger::setLoggerFile(const char *logFilename) {
	string str(logFilename);
	Logger::setLoggerFile(str);
}

Logger* Logger::getInstance() {
	if (Logger::_logger == NULL)
		Logger::_logger = new Logger();
	return Logger::_logger;
}
void Logger::log(const string msg) {
	_logMutex.lock();
	boost::posix_time::ptime now =
			boost::posix_time::second_clock::universal_time();
	if (Logger::_logFileSet) {
		ofstream outfile(Logger::_logFilename.c_str(), ios_base::app);
		outfile << boost::posix_time::to_simple_string(now).c_str()
				<< " UTC -> " << msg << endl;
		outfile.flush();
		outfile.close();
	} else {
		cout << boost::posix_time::to_simple_string(now).c_str() << " UTC -> "
				<< msg << endl;
	}
	_logMutex.unlock();
}
void Logger::log(const char* msg) {
	string str(msg);
	log(str);
}

Logger::Logger() :
	_logMutex() {
	if (Logger::_logFileSet) {
		ofstream outfile(Logger::_logFilename.c_str());
		outfile.close();
	}
}
Logger::~Logger() {
	;
}

}
}
