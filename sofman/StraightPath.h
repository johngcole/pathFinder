/*
 * StraightPath.h
 *
 *  Created on: Jun 10, 2012
 *      Author: gray
 */

#ifndef __STRAIGHTPATH_H_
#define __STRAIGHTPATH_H_

#include "Path.h"

class StraightPath : public Path {

public:
	StraightPath(Position3D start, Position3D end);
	StraightPath(Position3D start, NorthBearingAngle bearing, Length len);
	~StraightPath() {;}

    virtual Length length();
    virtual void fillPathError(Position3D currPos, NorthBearingAngle currHeading, PathError &error);
    virtual Position3D start();
    virtual Position3D end();
    virtual NorthBearingAngle startBearing();
    virtual NorthBearingAngle endBearing();
    virtual Position3D findPosition(Length dist);
    virtual Position3D findPosition(float percentage);

private:
    bool insideStraightPath(Position3D pos);
    bool isLeft( Position3D a, Position3D b, Position3D c );

	Position3D _start, _end;
	NorthBearingAngle _path_bearing;
};

#endif /* STRAIGHTPATH_H_ */
