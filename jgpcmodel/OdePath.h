#ifndef ODE_PATH__H__
#define ODE_PATH__H__

using namespace std;

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <vector>
#include <unistd.h>
#include <sys/time.h>

#include "car.h"
#include "OdePathIns.h"
#include "OdePathRadar.h"

#define MAXWAYPOINTS 10


class OdePath {

 public:
  OdePath(Car *ode);
  ~OdePath();
  
  static int loadWayPointFile(const char*, P2D *);
  OdePathIns *getPathIns() {return _pins;};
  //void setPathIns(OdePathIns *);
  static void *PathThread(void *args);
  OdePathRadar *getRadarSensor() {return _pradar;};
  P2D arcPoint(float, float, float, float); //dir, curr x, curr y, radius

 private:
  float getHeadingToPoint(P2D , P2D );
  float distanceBetween(P2D , P2D ); // from, to
  pthread_t _thread;
  Car *_ode;
  OdePathIns *_pins;
  OdePathRadar *_pradar;
};

#endif 



