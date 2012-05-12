#include "OdePath.h"


OdePath::OdePath(Car *ode) {
  _ode = ode;
  _pins = new OdePathIns();
   _pradar = new OdePathRadar();
  pthread_create(&_thread, NULL, PathThread, (void*)this);
}

OdePath::~OdePath() {;}


int OdePath::loadWayPointFile(const char* infile, P2D *wp) {
  int r = 0;
  // read in waypoints if file exits
  FILE *wpfile = fopen(infile,"r");
  if (wpfile == NULL) {
    printf("cannot open %s -- setting waypoints to 0\n",infile);
    //numWaypoints = 0;
    //wpcounter = -1;
    //exit(-1);
  } else {
    while (!feof(wpfile) && r < MAXWAYPOINTS) {
      fscanf(wpfile,"%f\t%f\n",
	     &wp[r].px,&wp[r].py);
      if (r < MAXWAYPOINTS)
	r++;
    }
    fclose(wpfile);
  }
  return r;

};


void* OdePath::PathThread(void* args) {
  OdePath *path = (OdePath*)args;
  timeval lastSample, now;
  double timeDiff;
  int numWaypoints;
  int wpcounter = -1;
  P2D waypoints[MAXWAYPOINTS];
  P2D destination, currentpos, prevpos;
  double pos[3];
  double att[3];
  P2D *wpp = waypoints;
  double testInterval = 0.15;
  float adjsteer;
  P2D wpsave;
  bool dodging = false;
  int delay = 0;
  // create a variable to see if we have seen this radar hit before
  int prevCval = 0;
  // need to change this when rebuilding with ray based radar
  bool fixedobsbuild = false;

  gettimeofday(&lastSample, NULL);
  numWaypoints = loadWayPointFile("waypoints.dat",wpp);
  // set destination to first waypoint 
  
  if (numWaypoints > 0) {
    destination.px = waypoints[0].px;
    destination.py = waypoints[0].py;
    wpcounter = 0;
  }
  
  

  while (1) {
    gettimeofday(&now, NULL);
    timeDiff = (now.tv_sec+(now.tv_usec/1000000.0)) - 
      (lastSample.tv_sec+(lastSample.tv_usec/1000000.0));
    
    // path testing interval
    if (timeDiff > testInterval) {
      // check the ins for position
      double *p = path->_pins->currentPos();
      double *a = path->_pins->currentAttitude();
      for (int i = 0; i < 3; i++) {
	pos[i] = p[i];
	att[i] = a[i];
      }
      currentpos.px = pos[0];
      currentpos.py = pos[1];

      // check the radar for obstacles
      // counter will always be larger if a new radar hit
      int counter = path->_pradar->getHitCount();
      if (counter > prevCval) {
	if (!fixedobsbuild)
	  prevCval = counter;
	p = path->_pradar->getHitPos();
	// see if obs is right or left of our line
	P2D ocontact;
	ocontact.px = p[0];
	ocontact.py = p[1];
	float ohead = path->getHeadingToPoint(currentpos, ocontact);
	/*
	if (path->distanceBetween(currentpos,ocontact) < 3.5) {
	  spd = path->_ode->getSpeed();
	  path->_ode->adjustSpeed(-0.001*spd);
	}
	*/
	float courseChange;
	// guard against 350 vs 10 in headings
	if (fabsf(att[2] - ohead) >= 45.0) {
	  if (ohead < 45.0) // obstacle is to the right
	    courseChange = -45.0;
	  else
	    courseChange = 45.0;
	} else {
	  courseChange = att[2] <= ohead ? -45.0 : 45.0;
	}
	// start avoidance manuvers
	dodging = true;
	wpsave = waypoints[wpcounter];
	// steer left -- simple avoidance
	destination = path->arcPoint(att[2] + courseChange,currentpos.px,currentpos.py,4.0);
	printf("OdePath: dodging obs at %f %f from %f to heading %f\n",p[0],p[1],
	   att[2],att[2] + courseChange);
	delay = 0;
      } else if (delay++ > 28) { // no obstacles quit dodging after short delay
	delay = 0;
	/*
	spd = path->_ode->getSpeed();
	if (spd < 5.0) 
	  path->_ode->adjustSpeed(1.1*spd);
	*/
	// this brings us back toward the obstacle too quickly need to keep dodging for 
	// a couple of extra counts
	if (dodging) {
	  destination = waypoints[wpcounter];
	}
	dodging = false;
	}
    }
    if (numWaypoints > 0) {
      // adjust speed and direction based on heading
      //printf("Thread print pos[0-2] %f, %f, %f\n",pos[0],pos[1],pos[2]);
      prevpos.px = currentpos.px;
      prevpos.py = currentpos.py;
      currentpos.px = pos[0];
      currentpos.py = pos[1];
      float heading = att[2];
      float dheading = path->getHeadingToPoint(currentpos, destination);

      float pcterror;
      float dhd = (dheading - heading) > 180.0 ? dheading - 360.0 : dheading;
      float hd = (heading - dheading) > 180.0 ? heading - 360.0 : heading;
      pcterror = (dhd - hd) / 360;
      pcterror *= 2.15; // step up adjust vs steer
      //printf("dheading - heading %f - %f  = %f\n",dheading,heading,pcterror);
      if (pcterror < -0.0139 || pcterror > 0.0139) { //  5degree error 
	// steer by amount of error for 5 iterations then return to straight
	adjsteer = pcterror;
	if (adjsteer < -0.9)
	  adjsteer = -0.9;
	else if (adjsteer > 0.9)
	  adjsteer = 0.9;
	path->_ode->adjustCourse(adjsteer);
      }

      // update the waypoint

      if (path->_ode->distanceRemaining(currentpos,destination) < 1.9  && !dodging) {
	if (wpcounter < numWaypoints-1) { 
	  // move to next waypoint
	  wpcounter++;
	  destination.px = waypoints[wpcounter].px;
	  destination.py = waypoints[wpcounter].py;
	  
	  printf("waypoint change wp %i %f %f cp %f %f\n",wpcounter,
		 waypoints[wpcounter].px,waypoints[wpcounter].py,
		 currentpos.px,currentpos.py);
	} else { // reached last waypoint wrap around to first one again
	  // stop
	  path->_ode->setSpeed(0.0);
	}	  
      } 
      
    }

    
    usleep(10);
  }

};

  

float OdePath::distanceBetween(P2D from, P2D to) {
  float dx, dy;
  dx = from.px - to.px;
  dy = from.py - to.py;
  return(sqrt(pow(dx,2) + pow(dy,2)));
};

float OdePath::getHeadingToPoint(P2D currentpos, P2D destination) {
  /***	 desired heading would be from current pos.y to dest.y
	 if dest.x > cur.x then use 90 - angle
	 tan(q) = a[0]-dest.px / a[1] - dest.py 
	 q = ...
	 desired heading = ...

	 4 quadrants again 

	 x < x1 && y < y1 dhead in upper left q  360 - y/x
	 x < x1 && y > y1 dhead in upper right q   y/x
	 x > x1 && y < y1 dhead in lower left q  180 + y/x
	 x > x1 && y > y1 dhead in lower right q  180 - y/y
	 

  */
  float dheading;
  if (currentpos.px < destination.px && currentpos.py < destination.py) {
    dheading = 360 - fabsf(atanf((currentpos.py-destination.py)/(currentpos.px-destination.px))*57.29578);
  } else  if (currentpos.px < destination.px && currentpos.py > destination.py) {
    dheading = fabsf(atanf((currentpos.py-destination.py)/(currentpos.px-destination.px))*57.29578);
  } else if (currentpos.px > destination.px && currentpos.py < destination.py) {
    dheading = 180 + fabsf(atanf((currentpos.py-destination.py)/(currentpos.px-destination.px))*57.29578);
  } else {
    dheading = 180 - fabsf(atanf((currentpos.py-destination.py)/(currentpos.px-destination.px))*57.29578);
  }    
  if (dheading > 360)
    dheading -= 360;
  return(dheading);

};


P2D OdePath::arcPoint(float deg, float x, float y, float r) {
  P2D pt;
  /* deg is the heading -- the angle in this formula is the angle between the 
     heading and the x axis, but since this entire world is turned 90 degrees left
     the angle will be the angle from the negative y axis and the line to the point
     on the circle, ie, heading of 360 will cause an angle of 90; 270 will be 180;
     180 (due negative on x) will be 270; 
   */
  float angle;
  // force degrees to be correct
  if (deg > 360.0)
    deg -=  360.0;
  else if (deg < 0.0)
    deg += 360.0;
  
  angle = 360.0 - deg;
  
  // cos and sin expect radians, so angle is mult by pi/180 to conv deg to rad
  pt.px = r * cos(angle*M_PI/180) + x;
  pt.py = r * sin(angle*M_PI/180) + y;
  //cx -= r;
  return pt;
};
