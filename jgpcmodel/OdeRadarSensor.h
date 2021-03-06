#ifndef ODE_RADAR_SENSOR__H__
#define ODE_RADAR_SENSOR__H__

using namespace std;

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <vector>
#include <unistd.h>
#include <sys/time.h>

#include "solidobstacle.h"
#include "RadarSubscriber.h"
#include "OdePathIns.h"
#include "radar.h"
#include "util.h"
// 2HZ
#define RADAR_SAMPLE_RATE   0.5

class OdeRadarSensor {

 public:
  OdeRadarSensor(Obstacles *obs);
  ~OdeRadarSensor();
  
  void addSubscription(RadarSubscriber *reader);
  void removeSubscription(RadarSubscriber *reader);
  OdePathIns *getPathIns() {return _pins;};
  void sensorReturn(int cval, double xpos, double ypos, double zpos);
  bool isScanning() {return _scanning;};
  
  static void *SampleThread(void *args);
  
 private:
  pthread_t _thread;
  Obstacles *_obs;
  OdePathIns *_pins;
  vector<RadarSubscriber*> _readers;
  double *_xpos;
  double *_ypos;
  double *_zpos;
  bool _scanning;
};

#endif 



