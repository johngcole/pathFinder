#include <iostream>

#include <colelib/Logger.h>
#include <colelib/Constants.h>

using namespace std;
using namespace cole::util;

int main() {

	Logger::setLoggerFile("./log.txt");
	Logger::getInstance()->log("");
	Logger::getInstance()->log("");
	Logger::getInstance()->log("    sofman cometh...  ");
	Logger::getInstance()->log("----------------------");


    return 1;
}


