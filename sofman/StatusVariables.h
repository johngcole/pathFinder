#ifndef __STATUSVARIABLES_H_
#define __STATUSVARIABLES_H_


#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>

#include <iostream>
#include <colelib/Position3D.h>
#include <colelib/Attitude.h>

#include "Path.h"

using namespace std;
using namespace cole::math::position;

class StatusVariables {
public:
	StatusVariables();
	~StatusVariables();

	void setCarPosition(Position3D &pos);
	void setCarAttitude(Attitude &att);

	Position3D getCarPosition();
	Attitude getCarAttitude();

	boost::shared_ptr<Path> getPath();
	void setPath(boost::shared_ptr<Path> path);

private:
	boost::mutex _carMutex;
	Position3D _carPos;
	Attitude _carAtt;

	boost::mutex _pathMutex;
	boost::shared_ptr<Path> _path;
};


#endif /* STATUSVARIABLES_H_ */
