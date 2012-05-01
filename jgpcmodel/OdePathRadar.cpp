#include "OdePathRadar.h"

void OdePathRadar::AcceptRADAR(int count, double x, double y, double z) {
  /*
  printf("RADAR EVENT FIRED: pos:(%0.3f,%0.3f,%0.3f)\n", 
	 x,y,z);
  */
  hitcount = count;
  pos[0] = x;
  pos[1] = y;
  pos[2] = z;
};
