#ifndef radar_h 
#define radar_h

#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include "texturepath.h"
#include <stdio.h>
#include "util.h"
#include "solidobstacle.h"
#include "jPoly.h"

/* Radar class in this version is implemented using the knowlege of the lo
   location of objects in the world.  It may be reimplemented later using
   rays to detect objects

   This implemetation requires an OBSTACLE class object to be provided
   to the RADAR object so that it can scan the locations of the obstacles
   and test to see if they have been detected

   After each scan the class with return an array  with the location of all
   obstacles that were detected in the scan
   
   whenever a scan is requested the radar unit will create a polygon arc
   for 40 degrees to the front -- the arc will look like a pie slice
   and all objects in the OBSTACLE pointer will be tested to see if they are
   in the polygon.  If yes they are added to the bogey array which is returned
   to the caller

 */

#define MAXSCANOBS 10
#define DEFLENGTH 4.5
#define DEFDIR 360
#define DEFARC 40.0
#define DEFFREQ 50
#define SCANLINES 5 // number of lines to draw to simulate scan

#define RADARDEBUG 1

class Radar {
 public:
  Radar(float len) {length = len; 
    dir = DEFDIR;
    arc = DEFARC;
    freq = DEFFREQ;
    scanning = false;
    scancnt = -1; 
    ocnt = 0;
    bogeyhits = -1;
    currentScan = new jPoly();
  };
  /* // ray version not implemented yet
  Radar(dSpaceID sp, float len, float heading) { 
    space = sp;
    length = len;
    dir = heading;
    ready = 1;
    scanning = false;
  };
  void init();
  */
  void setLength(float len) {length = len;};
  void setDir(float heading) {dir = heading;};
  void setArc(float angle) {arc = angle;};
  void setFrequency(int cnt) {freq = cnt;};
  int getFrequency() {return freq;};
  float getDir() {return dir;};
  float getLength() {return length;}
  float getArc() {return arc;};
  void connectObstacles(Obstacles *ob) {sobs = ob; ocnt = ob->getSize();};
  int getHitCount() {return bogeyhits;};
  bool isScanning() {return scanning;};
  //void setSpace(dSpaceID sp) {space = sp;};
  void draw(float); // starting height of scan
  P2D arcPoint(float, float, float, float); //dir, curr x, curr y, radius
  int scan(BOGEY *, P2D, float); // position and heading of scan center
 protected:
  int scancnt, ocnt, bogeyhits;
  float min(float v1, float v2) {return v1 < v2 ? v1 : v2;};
  float max(float v1, float v2) {return v1 > v2 ? v1 : v2;};
  float distanceBetween(P2D , P2D ); // from, to
  bool segmentsIntersect(P2D *);
  float getHeadingToPoint(P2D , P2D );
  bool headingBetween(float , float, float ); // left, right, dir
  void circleTestPoints(float, float, float, float, P2D*); //heading,radius,cx,cy,p2d array to receive the points
  //dGeomID ray;
  float length;
  float dir; // center of the scan
  float arc; // width of scan
  int freq; // frequency of scan in ode steps
  bool scanning;
  Obstacles *sobs;
  jPoly *currentScan;
  BOGEY hits[MAXSCANOBS];
};

#endif
