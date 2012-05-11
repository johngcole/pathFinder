#include "OdeRadarSensor.h"

OdeRadarSensor::OdeRadarSensor(Obstacles *obs) {
  _obs = obs;
  _readers.clear();
  // create the pathins needed to get location for radar scans
  _pins = new OdePathIns();
  _scanning = true;
  // initialize lastRadarHit
  /*
  for (int i = 0; i < 3; i++)
    _lastRadarHit[i] = 0.0;
  */
  pthread_create(&_thread, NULL, SampleThread, (void*)this);
}

OdeRadarSensor::~OdeRadarSensor() {;}

void OdeRadarSensor::addSubscription(RadarSubscriber *reader) {
  // first, make sure this isn't in our list
  vector<RadarSubscriber*>::iterator i;
  for (i=_readers.begin(); i!=_readers.end(); ++i)
    if (*i == reader)
      return;
  _readers.push_back(reader);
}

void OdeRadarSensor::removeSubscription(RadarSubscriber *reader) {
  // find it in the list
  vector<RadarSubscriber*>::iterator i;
  for (i=_readers.begin(); i!=_readers.end(); ++i) {
    if (*i == reader) {
      _readers.erase(i);
      return;
    }
  }
}

void OdeRadarSensor::sensorReturn(int cval, double xpos, double ypos, double zpos) {
  vector<RadarSubscriber*>::iterator i;
  printf("OdeRadarSensor: sensorReturn %d, %f, %f. %f\n",cval,xpos,ypos,zpos);
  
  for (i=_readers.begin(); i!=_readers.end(); ++i) {
	(*i)->AcceptRADAR(cval, xpos, ypos, zpos);
      }
  
  // convert to heading and distance
  /*
  double heading = radunit->getHeadingDegrees();
  dMatrix3 rpos = radunit->getPosition(); 
  dP2D origin, rhit;
  origin.px = rpos[0];
  origin.py = rpos[1];
  rhit.px = xpos;
  rhit.py = ypos;
  double dist = radunit->distanceBetween(origin,rhit);
  */
  /* // version that returns distance and bearing
  for (i=radar->_readers.begin(); i!=radar->_readers.end(); ++i) {
    (*i)->AcceptRADAR(cval, dist, heading);
  */
  // temporary return version 
  /*
  for (i=radar->_readers.begin(); i!=radar->_readers.end(); ++i) {
    (*i)->AcceptRADAR(cval, xpos, ypos, zpos);
  }
  */
}

void* OdeRadarSensor::SampleThread(void* args) {
  OdeRadarSensor *radar = (OdeRadarSensor*)args;
  timeval lastSample, now;
  double timeDiff;
  double vals[3];
  int cval;
  double pos[3];
  double heading;
  
  Radar *radunit;
  P2D cpos; 
  BOGEY rhits[10];

  vector<RadarSubscriber*>::iterator i;

  gettimeofday(&lastSample, NULL);

  cval = 0;
  vals[0] = 0.0;
  vals[1] = 0.0;
  vals[2] = 0.0;

  // create the radar unit
  radunit = new Radar(5.0);
  radunit->connectObstacles(radar->_obs);

  while (1) {
    gettimeofday(&now, NULL);
    timeDiff = (now.tv_sec+(now.tv_usec/1000000.0)) - 
      (lastSample.tv_sec+(lastSample.tv_usec/1000000.0));
    if (timeDiff >= RADAR_SAMPLE_RATE) {
      // call the radar op here
      double *p = radar->_pins->currentPos();
      double *a = radar->_pins->currentAttitude();
      pos[0] = p[0];
      pos[1] = p[1];
      heading = a[2];
      cpos.px = p[0];
      cpos.py = p[1];
      
      //cval = radunit->scan(rhits,cpos,heading);
      if (cval > 0) {
	vals[0] = rhits[0].px;
	vals[1] = rhits[0].py;
	vals[2] = rhits[0].pheight;
	//printf("OdeRadarSensor: %i from %i at %f %f\n",cval,rhits[0].identifier,vals[0],vals[1]);
      } else {
	vals[0] = 0.0;
	vals[1] = 0.0;
	vals[2] = 0.0;
      }
      //radar->_ode->gatherPositionAndAttitude(vals);
      /*
      for (i=radar->_readers.begin(); i!=radar->_readers.end(); ++i) {
	(*i)->AcceptRADAR(cval, vals[0], vals[1], vals[2]);
      }
      */
      gettimeofday(&lastSample, NULL);
    }
    usleep(10);
  }

}

  
