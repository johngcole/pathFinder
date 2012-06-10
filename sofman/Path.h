/*
 * Path.h
 *
 *  Created on: Jun 10, 2012
 *      Author: gray
 */

#ifndef __PATH_H_
#define __PATH_H_

#include <colelib/Length.h>
#include <colelib/Angle.h>
#include <colelib/NorthBearingAngle.h>
#include <colelib/Position3D.h>

using namespace cole::math;
using namespace cole::math::position;

/* Path Error
 * Used as a mechanism to return both distance from Path and Bearing from Path
 *
 * DistanceFromPath - negative values indicate areas left of path
 * BearingFromPath - negative values indicate headings left of path's heading
 */
typedef struct path_error_t {
	Length DistanceFromPath;
	Angle BearingFromPath;
	NorthBearingAngle BearingAtPath;
} PathError;

class Path {
public:

    /* length()
     * Function returns the entire length of the path
     */
    virtual Length length() = 0;

    /* getPathError(AbsolutePosition pos)
     * Function returns the distance from Path, along with bearing from Path
     * NOTE: function will return false if any error occurs
     */
    virtual void fillPathError(Position3D currPos, NorthBearingAngle currHeading, PathError &error) = 0;

    /* start()
     * Function returns the starting location of a path
     */
    virtual Position3D start() = 0;

    /* end()
     * Function returns the ending location of a path
     */
    virtual Position3D end() = 0;

    /* startBearing()
     * Function returns the start bearing location of a path
     */
    virtual NorthBearingAngle startBearing() = 0;

    /* endBearing()
     * Function returns the end bearing location of a path
     */
    virtual NorthBearingAngle endBearing() = 0;

    /* findPosition()
     * Function returns the location of a path "dist" into path
     * NOTE: if dist<0, function will return start()
     * NOTE: if dist>length(), function will return end()
     */
    virtual Position3D findPosition(Length dist) = 0;

    /* findPosition()
     * Function returns the location of a path "percentage" into path
     * NOTE: if percentage<0, function will return start()
     * NOTE: if percentage>1.0, function will return end()
     */
    virtual Position3D findPosition(float percentage) = 0;

};

#endif /* __PATH_H_ */
