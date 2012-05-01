#include "OdePathIns.h"

void OdePathIns::AcceptINS(double roll, double pitch, double yaw,
			   double x, double y, double z) {
  /*
  printf("EVENT FIRED: pos:(%0.3f,%0.3f,%0.3f) att:(%0.3f,%0.3f,%0.1f)\n", 
	 x,y,z,roll,pitch,yaw);
  */
  pos[0] = x;
  pos[1] = y;
  pos[2] = z;
  attitude[0] = roll;
  attitude[1] = pitch;
  attitude[2] = yaw;
}
