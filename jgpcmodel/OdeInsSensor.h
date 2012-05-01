#ifndef ODE_INS_SENSOR__H__
#define ODE_INS_SENSOR__H__

using namespace std;

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <vector>
#include <unistd.h>
#include <sys/time.h>

#include "car.h"
#include "InsSubscriber.h"


// 2HZ
#define INS_SAMPLE_RATE   0.5

class OdeInsSensor {

 public:
  OdeInsSensor(Car *ode);
  ~OdeInsSensor();
  
  void addSubscription(InsSubscriber *reader);
  void removeSubscription(InsSubscriber *reader);
  
  static void *SampleThread(void *args);
  
 private:
  pthread_t _thread;
  Car *_ode;
  vector<InsSubscriber*> _readers;
};

#endif 



