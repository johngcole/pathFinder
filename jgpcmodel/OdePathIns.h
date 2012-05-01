#ifndef ODE_PATH_INS__H__
#define ODE_PATH_INS__H__

#include <stdio.h>
#include <stdlib.h>

#include "InsSubscriber.h"

class OdePathIns : public InsSubscriber {
 public:
  OdePathIns() {;}
  ~OdePathIns() {;}

  virtual void AcceptINS(double roll, double pitch, double yaw,
			 double x, double y, double z);

  double *currentPos() {return pos;};
  double *currentAttitude() {return attitude;};
 protected:
  double pos[3];
  double attitude[3];
};

#endif

