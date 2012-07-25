/*
 * CompositePath.h
 *
 *  Created on: Jul 25, 2012
 *      Author: gray
 */

#ifndef __COMPOSITEPATH_H_
#define __COMPOSITEPATH_H_

#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <colelib/Logger.h>

#include <float.h>

#include "Path.h"

class CompositePath : public Path {

public:
	CompositePath(vector< boost::shared_ptr<Path> > paths);
	~CompositePath() {;}

    virtual Length length();
    virtual void fillPathError(Position3D currPos, NorthBearingAngle currHeading, PathError &error);
    virtual Position3D start();
    virtual Position3D end();
    virtual NorthBearingAngle startBearing();
    virtual NorthBearingAngle endBearing();
    virtual Position3D findPosition(Length dist);
    virtual Position3D findPosition(float percentage);

private:
    vector< boost::shared_ptr<Path> > _paths;

};


#endif /* COMPOSITEPATH_H_ */
