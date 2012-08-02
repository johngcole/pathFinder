#include <boost/shared_ptr.hpp>

#include <iostream>

#include <colelib/Logger.h>
#include <colelib/Config.h>
#include <colelib/Constants.h>
#include "StatusVariables.h"
#include "DrawstuffGraphics.h"
#include "ODECar.h"
#include "ManualSteering.h"
#include "Path.h"
#include "CompositePath.h"
#include "StraightPath.h"
#include "ArcPath.h"

using namespace std;
using namespace cole::util;

boost::shared_ptr<Path> readConfigPath(Config *config);

int main() {

	// uncomment next line to use logfile, otherwise will use stdout
	//Logger::setLoggerFile("./log.txt");

	Logger::getInstance()->log("");
	Logger::getInstance()->log("");
	Logger::getInstance()->log("    sofman cometh...  ");
	Logger::getInstance()->log("----------------------");

	Config *config = new Config("./config.txt");
	StatusVariables *status = new StatusVariables();
	status->setPath(readConfigPath(config));
	Graphics *graphics = new DrawstuffGraphics(config, status);
	ODECar *car = new ODECar(config, status);
	ManualSteering *steer = new ManualSteering(car);

	graphics->AttachODECar(car);
	graphics->AttachKeyListener(steer);
	graphics->Start();
	car->Start();

	Logger::getInstance()->log("Starting statistics...");
	status->startStats();

	graphics->Join();  // wait on graphics, so user can hit "Escape"

	car->Stop();

	boost::posix_time::ptime t2 = boost::posix_time::microsec_clock::local_time();
	boost::posix_time::time_duration diff = t2 - status->getStartTime();

	Logger::getInstance()->log("*** Trip Statistics ****");
	char out[270];
	sprintf(out,"elapsed time %i total error %10.2f total distance %8.2f path length %5.2f",
	       diff.total_seconds(),
	       sqrt(status->getErrorValue()),
	       status->getDistanceTraveled(),
	       status->getPath()->length().getDoubleValue(Length::METERS));
	Logger::getInstance()->log(out);
	sprintf(out,"meas. cnt %i; error / cnt %6.4f; actual 2 std dist %3.2f actual 2 std time %3.2f\n",
	       status->getMeasurementCount(),
	       (sqrt(status->getErrorValue()))/(status->getMeasurementCount()),
	       status->getDistanceTraveled() / status->getPath()->length().getDoubleValue(Length::METERS),
	       (float) diff.total_seconds() / 21.5);
	Logger::getInstance()->log(out);

	delete steer;
	delete car;
	delete graphics;
	delete status;
	delete config;

    return 1;
}

boost::shared_ptr<Path> readConfigPath(Config *config) {
	vector< boost::shared_ptr<Path> > paths;
	ostringstream oss;
	Length x, y, z;

	int numPaths = config->getValueAsInt("NumPathPieces");
	if (numPaths < 0)
		throw ConfigException("NumPathPieces must be positive!");

	oss.str("");
	oss << "Reading " << numPaths << " path pieces...";
	Logger::getInstance()->log(oss.str());

	for (int i=0; i<numPaths; i++) {
		oss.str("");
		oss << "Path" << i << "_Type";
		string type = config->getValue(oss.str());
		if (type == "ArcPath") {
			// need to read start location
			oss.str("");
			oss << "Path" << i << "_StartX";
			x = Length(config->getValueAsDouble(oss.str()), Length::METERS );
			oss.str("");
			oss << "Path" << i << "_StartY";
			y = Length(config->getValueAsDouble(oss.str()), Length::METERS );
			oss.str("");
			oss << "Path" << i << "_StartZ";
			z = Length(config->getValueAsDouble(oss.str()), Length::METERS );
			Position3D start(x, y, z);

			// next, need radius
			oss.str("");
			oss << "Path" << i << "_Radius";
			Length radius( config->getValueAsDouble(oss.str()), Length::METERS );

			// next, need starting bearing
			oss.str("");
			oss << "Path" << i << "_StartBearing";
			NorthBearingAngle startBearing( config->getValueAsDouble(oss.str()), Angle::DEGREES );

			// next, need length of arc
			oss.str("");
			oss << "Path" << i << "_ArcLength";
			Length len( config->getValueAsDouble(oss.str()), Length::METERS );

			// finally, is it clockwise?
			oss.str("");
			oss << "Path" << i << "_Clockwise";
			bool clock = config->getValueAsBool(oss.str());

			boost::shared_ptr<Path> arcpath( new ArcPath(radius, start, startBearing, len, clock) );
			paths.push_back(arcpath);

		} else if (type == "StraightPath") {
			// need to read start location
			oss.str("");
			oss << "Path" << i << "_StartX";
			x = Length(config->getValueAsDouble(oss.str()), Length::METERS );
			oss.str("");
			oss << "Path" << i << "_StartY";
			y = Length(config->getValueAsDouble(oss.str()), Length::METERS );
			oss.str("");
			oss << "Path" << i << "_StartZ";
			z = Length(config->getValueAsDouble(oss.str()), Length::METERS );
			Position3D start(x, y, z);

			// now, end location
			oss.str("");
			oss << "Path" << i << "_EndX";
			x = Length(config->getValueAsDouble(oss.str()), Length::METERS );
			oss.str("");
			oss << "Path" << i << "_EndY";
			y = Length(config->getValueAsDouble(oss.str()), Length::METERS );
			oss.str("");
			oss << "Path" << i << "_EndZ";
			z = Length(config->getValueAsDouble(oss.str()), Length::METERS );
			Position3D end(x, y, z);

			boost::shared_ptr<Path> strpath( new StraightPath(start, end) );
			paths.push_back(strpath);

		} else {
			oss.str("");
			oss << "ERROR: Unknown Path type: " << type;
			Logger::getInstance()->log(oss.str());
		}
	}

	oss.str("");
	oss << "Creating composite path with " << paths.size() << " parts...";
	Logger::getInstance()->log(oss.str());

	boost::shared_ptr<Path> thepath( new CompositePath(paths) );
	return thepath;
}


/******  PATH TESTING
	Position3D start(Length(0.0, Length::METERS), Length(0.0, Length::METERS), Length(0.0, Length::METERS));
	Position3D end(Length(5.0, Length::METERS), Length(5.0, Length::METERS), Length(0.0, Length::METERS));
	boost::shared_ptr<Path> path1( new StraightPath(start, end) );

	Length radius(5.0, Length::METERS);
	Length len(15.708, Length::METERS);
	NorthBearingAngle startBearing(45.0, Angle::DEGREES);
	boost::shared_ptr<Path> path2( new ArcPath(radius, end, startBearing, len, false) );

	vector< boost::shared_ptr<Path> > paths;
	paths.push_back(path1);
	paths.push_back(path2);
	boost::shared_ptr<Path> path( new CompositePath(paths) );

	cout << "Path start: " << path->start().toString() << endl;
	cout << "Path end: " << path->end().toString() << endl;
	cout << "Path Length: " << path->length().toString() << endl;
	cout << "Path Start Bearing: " << path->startBearing().correctFullCircle().toString() << endl;
	cout << "Path End Bearing: " << path->endBearing().correctFullCircle().toString() << endl;
	cout << "Position(.25): " << path->findPosition(.25).toString() << endl;
	cout << "Position(.5): " << path->findPosition(.5).toString() << endl;
	cout << "Position(.75): " << path->findPosition(.75).toString() << endl;

	PathError pe;
	Position3D test(Length(2.0, Length::METERS), Length(2.5, Length::METERS), Length(0.0, Length::METERS));
	NorthBearingAngle heading(20, Angle::DEGREES);
	path->fillPathError(test, heading, pe);
	cout << endl;
	cout << "Test Point: " << test.toString() << endl;
	cout << "Test Heading: " << heading.toString() << endl;

	cout << "Path Error:" << endl;
	cout << "\tDist: " << pe.DistanceError.toString() << endl;
	cout << "\tBearingAtPath: " << pe.BearingAtPath.toString() << endl;
	cout << "\tBearingError: " << pe.BearingError.toString() << endl;
 */

