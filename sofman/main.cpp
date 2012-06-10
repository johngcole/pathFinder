#include <iostream>

#include <colelib/Logger.h>
#include <colelib/Config.h>
#include <colelib/Constants.h>
#include "StatusVariables.h"
#include "DrawstuffGraphics.h"
#include "ODECar.h"
#include "ManualSteering.h"

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
	ODECar *car = new ODECar(config, status);
	ManualSteering *steer = new ManualSteering(car);

	graphics->AttachODECar(car);
	graphics->AttachKeyListener(steer);
	graphics->Start();
	car->Start();

	boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
	Logger::getInstance()->log(">>>>> Speed: 0.5");
	car->setCarSpeed(0.5f);

	graphics->Join();  // wait on graphics, so user can hit "Escape"

	car->Stop();

	delete steer;
	delete car;
	delete graphics;
	delete status;
	delete config;
    return 1;
}


