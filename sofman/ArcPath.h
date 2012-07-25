/*
 * ArcPath.h
 *
 *  Created on: Jul 25, 2012
 *      Author: gray
 */

#ifndef __ARCPATH_H_
#define __ARCPATH_H_

#include "Path.h"

class ArcPath : public Path {

public:
	ArcPath(Position3D center, Length radius, NorthBearingAngle start, NorthBearingAngle end, bool clockwise);
	ArcPath(Length radius, Position3D startLocation, NorthBearingAngle startBearing, Length length, bool clockwise);
	~ArcPath() {;}

    virtual Length length();
    virtual void fillPathError(Position3D currPos, NorthBearingAngle currHeading, PathError &error);
    virtual Position3D start();
    virtual Position3D end();
    virtual NorthBearingAngle startBearing();
    virtual NorthBearingAngle endBearing();
    virtual Position3D findPosition(Length dist);
    virtual Position3D findPosition(float percentage);

private:
    bool isBearingWithinArcPath(NorthBearingAngle bearing);

    Position3D _center;
    Length _radius;
    NorthBearingAngle _arc_start, _arc_end;
    bool _clockwise;
    Angle _arc_len;
};


#endif /* ARCPATH_H_ */
