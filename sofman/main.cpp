#include <iostream>

#include <colelib/Logger.h>
#include <colelib/Config.h>
#include <colelib/Constants.h>
#include "StatusVariables.h"

using namespace std;
using namespace cole::util;

int main() {

	Logger::setLoggerFile("./log.txt");
	Logger::getInstance()->log("");
	Logger::getInstance()->log("");
	Logger::getInstance()->log("    sofman cometh...  ");
	Logger::getInstance()->log("----------------------");

	Config *config = new Config("./config.txt");
	StatusVariables *status = new StatusVariables();



	delete status;
	delete config;
    return 1;
}


