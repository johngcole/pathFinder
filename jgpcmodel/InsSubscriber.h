#ifndef INS_SUBSCRIBER__H__
#define INS_SUBSCRIBER__H__

class InsSubscriber {
 public:
  virtual void AcceptINS(double roll, double pitch, double yaw, 
			 double x, double y, double z) {;}
};

#endif

