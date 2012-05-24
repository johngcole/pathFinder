#include <iostream>

#include <colelib/Logger.h>
#include <colelib/Config.h>
#include <colelib/Constants.h>
#include "StatusVariables.h"
#include "DrawstuffGraphics.h"

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
	Graphics *graphics = new DrawstuffGraphics(config, status);

	graphics->Start();
	graphics->Join();

	delete status;
	delete config;
    return 1;
}


