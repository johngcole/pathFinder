#ifndef jPoly_h
#define jPoly_h

/** jPoly Class to hold a single polygon and the method to test whether a real world point
 *  is inside of it
 *
 *  @author John G. Cole
 *  @author Gray Cole
 *
 *  @version 1.1

 taken from the java version written in 2000
 */

#include "util.h"
#include <stdio.h>

#define JPDEBUG 0
#define MAXVERTICES 150

class jPoly {
 public:
  jPoly() {valid = false;};
  jPoly(jPoly *a) {numPoints = a->numPoints; bbox = a->bbox; 
    for (int i = 0; i < numPoints; i++)
      points[i] = a->points[i];
    valid = a->valid;};
  jPoly(int , P2D *);  
  bool isValid() { return valid;}
  bool pointInPoly(P2D );
  int numPoints;
  P2D points[150];
  jRect bbox;	
  bool valid ;
 protected:
  bool approxEqual(float , float );
  /** variable to reflect whether poly points are clockwise or counterclockwise
   *  is used by shape file to mean excluded poly when counter
   */
  bool clockwise;

};

#endif
