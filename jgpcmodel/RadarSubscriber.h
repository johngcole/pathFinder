#ifndef RADAR_SUBSCRIBER__H__
#define RADAR_SUBSCRIBER__H__

class RadarSubscriber {
 public:
  virtual void AcceptRADAR(int count, double x, double y, double z) {;}
};

#endif

