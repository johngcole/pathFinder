#include "OdeInsSensor.h"

OdeInsSensor::OdeInsSensor(Car *ode) {
  _ode = ode;
  _readers.clear();
  pthread_create(&_thread, NULL, SampleThread, (void*)this);
}

OdeInsSensor::~OdeInsSensor() {;}

void OdeInsSensor::addSubscription(InsSubscriber *reader) {
  // first, make sure this isn't in our list
  vector<InsSubscriber*>::iterator i;
  for (i=_readers.begin(); i!=_readers.end(); ++i)
    if (*i == reader)
      return;
  _readers.push_back(reader);
}

void OdeInsSensor::removeSubscription(InsSubscriber *reader) {
  // find it in the list
  vector<InsSubscriber*>::iterator i;
  for (i=_readers.begin(); i!=_readers.end(); ++i) {
    if (*i == reader) {
      _readers.erase(i);
      return;
    }
  }
}

void* OdeInsSensor::SampleThread(void* args) {
  OdeInsSensor *ins = (OdeInsSensor*)args;
  timeval lastSample, now;
  double timeDiff;
  double vals[6];
  vector<InsSubscriber*>::iterator i;

  gettimeofday(&lastSample, NULL);

  while (1) {
    gettimeofday(&now, NULL);
    timeDiff = (now.tv_sec+(now.tv_usec/1000000.0)) - 
      (lastSample.tv_sec+(lastSample.tv_usec/1000000.0));
    if (timeDiff >= INS_SAMPLE_RATE) {
      ins->_ode->gatherPositionAndAttitude(vals);
      for (i=ins->_readers.begin(); i!=ins->_readers.end(); ++i) {
	(*i)->AcceptINS(vals[0], vals[1], vals[2], 
			vals[3], vals[4], vals[5]);
      }
      gettimeofday(&lastSample, NULL);
    }

    usleep(10);
  }

}

  
