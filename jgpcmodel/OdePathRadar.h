#ifndef ODE_PATH_RADAR__H__
#define ODE_PATH_RADAR__H__

#include <stdio.h>
#include <stdlib.h>

#include "RadarSubscriber.h"

class OdePathRadar : public RadarSubscriber {
 public:
  OdePathRadar() {hitcount = 0;}
  ~OdePathRadar() {;}

  virtual void AcceptRADAR(int count, double x, double y, double z);

  double *getHitPos() {return pos;};
  int getHitCount() {return hitcount;};
 protected:
  int hitcount;
  double pos[3];
  //double heading;
};

#endif

