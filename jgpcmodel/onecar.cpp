/*************************************************************************
 *                                                                       *
 * Open Dynamics Engine, Copyright (C) 2001,2002 Russell L. Smith.       *
 * All rights reserved.  Email: russ@q12.org   Web: www.q12.org          *
 *                                                                       *
 * This library is free software; you can redistribute it and/or         *
 * modify it under the terms of EITHER:                                  *
 *   (1) The GNU Lesser General Public License as published by the Free  *
 *       Software Foundation; either version 2.1 of the License, or (at  *
 *       your option) any later version. The text of the GNU Lesser      *
 *       General Public License is included with this library in the     *
 *       file LICENSE.TXT.                                               *
 *   (2) The BSD-style license that is included with this library in     *
 *       the file LICENSE-BSD.TXT.                                       *
 *                                                                       *
 * This library is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the files    *
 * LICENSE.TXT and LICENSE-BSD.TXT for more details.                     *
 *                                                                       *
 *************************************************************************/

/*

buggy with suspension.
this also shows you how to use geom groups.

destimation read from text file

version 1.1 change vehicle to 4 wheels
version 1.2 add the tube car
add obstacle class object
add car as class object
move waypoints and commands to car object
add tower object
add a ray to the nose of the car to observe its collisions

*/


#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include "texturepath.h"
#include <stdio.h>
#include <math.h>

#include "solidobstacle.h"
#include "car.h"
#include "jPoly.h"
#include "util.h"
#include "OdePath.h"
#include "OdeInsSensor.h"
#include "OdeRadarSensor.h"
#include "OdePathIns.h"

// some constants

#define LENGTH 0.7	// chassis length
#define WIDTH 0.5	// chassis width
#define HEIGHT 0.2	// chassis height
#define RADIUS 0.18	// wheel radius


// starting height of chassis when dropped at beginning of simulation 
//  original value = 0.5
#define STARTZ 0.5	// starting height of chassis


#define DRAW_RADAR 1
#define DEBUG 0
#define DEBUG2 0

// permanent structures to build

// dynamics and collision objects (chassis, 3 wheels, environment)

static dWorldID world;
static dSpaceID space;
static dJointGroupID contactgroup;
static dGeomID ground;

int subcnt = -1;

int counter = 1;

Obstacles *obs;
Car *c1;
Car *car2follow = NULL;

// radian values for the scan sweep
//double scan[5] = {-0.349066,-0.174533,0.0,0.174533,0.349066};  /40 degrees
//double scan[5] = {-0.174533,-0.087266,0.0,0.087266,0.174533};   // 20 degrees
double scan[7] = {-0.30543262,-0.20362233,-0.10181029,0.0,0.10181029,0.20362233,0.30543262};   // 35 degrees
int spos = 3;
int sdir = -1;

OdeInsSensor *ins;
OdeRadarSensor *rad;
OdePath *path;
//OdePathIns *_pins;

// ray for testing
static dGeomID ray;


// things that the user controls

P2D arcPoint(float deg, float x, float y, float r) {
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
}


float angleAdjustment(P2D from, P2D to) {
  float dx, dy,dz;
  dx = abs(from.px - to.px);
  dy = abs(from.py - to.py);
  dz = sqrt(pow(dx,2) + pow(dy,2));
  // opposite is dy
  return(asin(dy/dz)*180/M_PI);
}

// this is called by dSpaceCollide when two objects in space are
// potentially colliding.

static void nearCallback (void *data, dGeomID o1, dGeomID o2)
{
  int i,n;
  const int N = 10;
  dContact contact[N];
  //int class1,class2;
  // we want all collisions

  dBodyID b1 = dGeomGetBody(o1);
  dBodyID b2 = dGeomGetBody(o2);
  if (b1 && b2 && dAreConnected(b1, b2))
    return;

  // ray class test
  dContactGeom rhit;
  if (dGeomGetClass (o1) == dRayClass || dGeomGetClass (o2) == dRayClass) {
    if (dCollide (o1,o2,1,&rhit,sizeof(dContactGeom))) {
      if (rad->isScanning()) {
	rad->sensorReturn(counter,rhit.pos[0],rhit.pos[1],rhit.pos[2]);
      } else {
	printf("ray collision at %f,%f,%f\n", rhit.pos[0],rhit.pos[1],rhit.pos[2]);
      }
    }
  } else {
    n = dCollide (o1,o2,N,&contact[0].geom,sizeof(dContact));
    if (n > 0) {
      for (i=0; i<n; i++) {
	contact[i].surface.mode = dContactSlip1 | dContactSlip2 |
	  dContactSoftERP | dContactSoftCFM | dContactApprox1;
      
	// jgc adj values -- see contact in manual for descrip
	contact[i].surface.mu = 0.95;
	contact[i].surface.slip1 = 0.05;
	contact[i].surface.slip2 = 0.05;
	contact[i].surface.soft_erp = 0.5;
	contact[i].surface.soft_cfm = 0.01;

	dJointID c = dJointCreateContact (world,contactgroup,&contact[i]);
	//dJointID c = dJointCreateContact (world,contactgroup,contact+i);
	dJointAttach (c,
		      dGeomGetBody(contact[i].geom.g1),
		      dGeomGetBody(contact[i].geom.g2));
      }
     
    }
  }
}


// start simulation - set viewpoint

static void start() {
  dAllocateODEDataForThread(dAllocateMaskAll);

  static float xyz[3] = {1.317f,-1.9817f,0.8000f};
  static float hpr[3] = {100.0000f,-27.5000f,0.0000f};
  dsSetViewpoint (xyz,hpr);
}


// called when a key pressed

static void command (int cmd) {
}


// simulation loop

static void simLoop (int pause) {

  if (car2follow != NULL && counter % 125 == 0) {
    const dReal *t = car2follow->getCarPos();
    float abc[3] = {t[0]-5.0+1.347,t[1]-1.0-2.0213,2.8800};
    float def[3] = {44.0000f,-29.0000f,0.0000f};
    if (DEBUG)
      printf("resetting viewpoint at %i x,y %f,%f\n",counter,t[0],t[1]);
    dsSetViewpoint (abc,def);
  }  
  if (!pause) {
    // catch any speed / steer changes from keyboard
    c1->step();
    dSpaceCollide (space,0,&nearCallback);
    dWorldStep (world,0.05);
    // remove all contact joints
    dJointGroupEmpty (contactgroup);
  }
  // maroon
  //dsSetColor (0.3137,0,0);
  //dsSetColor (0.8,0.3333,0);
  dsSetTexture (DS_WOOD);
  
  c1->draw();
  //c2->draw();

  dMatrix3 candir;
  const dReal* cpos = c1->getCarPos();
  float hdgDeg = c1->getHeading();
  // move the scan line
  if (counter % 5 == 0) { 
    spos += sdir;
    if (spos < 0) {
      spos = 1;
      sdir = 1;
    } else if (spos > 6) {
      spos = 5;
      sdir = -1;
    }
  }
  // 5th field is direction in radians  
  dRFromAxisAndAngle (candir,0,0,1,(hdgDeg*M_PI/180.0)+scan[spos]);
  // to set the ray in front of the car so that it does not collide with car
  P2D cpt = arcPoint(hdgDeg,cpos[0],cpos[1],LENGTH/2.0);
  dGeomRaySet (ray,cpt.px,cpt.py,0.10,candir[0],candir[1],candir[2]);
  if (DRAW_RADAR) {
    dVector3 origin,dir;
    dGeomRayGet (ray,origin,dir);
    dReal length = dGeomRayGetLength (ray);
    for (int j=0; j<3; j++) dir[j] = dir[j]*length + origin[j];
    dsDrawLine (origin,dir);
  }

  
  obs->draw();

  dsSetColor (1,1,1);


  counter++;

}


int main (int argc, char **argv) {
  FILE *ofile;
  dMass m;

  // setup pointers to drawstuff callback functions
  dsFunctions fn;
  fn.version = DS_VERSION;
  fn.start = &start;
  fn.step = &simLoop;
  fn.command = &command;
  fn.stop = 0;
  fn.path_to_textures = DRAWSTUFF_TEXTURE_PATH;

  // create world
  dInitODE2(0);
  world = dWorldCreate();
  space = dHashSpaceCreate (0);
  contactgroup = dJointGroupCreate (0);
  dWorldSetGravity (world,0,0,-0.5);
  ground = dCreatePlane (space,0,0,1,0);



  // environment

  obs = new Obstacles();
  obs->loadFromFile("obstacles.dat");
  obs->setTypeColor(1,0xFFFF00);
  obs->setTypeColor(3,0x00C01E);
  obs->setTypeColor(6,0x00A600);
  obs->build(world, space, m);


  c1 = new Car(LENGTH,WIDTH,HEIGHT,RADIUS);
  c1->setSpace(space);
  c1->makeCar(world,0.0, 0.0,STARTZ);
  c1->setBodyColor(0,0x800000);
  c1->loadCommandFile("test.csv");
  //c1->loadWayPointFile("waypoints.dat");
  ofile = fopen("c1Track.csv","w");
  if (ofile == NULL) 
    printf("Could not open output file\n");
  else
    c1->recordTrack(ofile);

  // create ray on front of car
  ray = dCreateRay(space,5.0);
  dMatrix3 candir;
  const dReal* cpos = c1->getCarPos();
  const dReal* catt = c1->getCarAttitude();
  // 5th field is direction in radians
  dRFromAxisAndAngle (candir,0,0,1,catt[2]);
  dGeomRaySet (ray,cpos[0]+WIDTH/2.0,cpos[1]+LENGTH/2.0,0.15,candir[0],candir[1],candir[2]);

  car2follow = c1;

  ins = new OdeInsSensor(c1);
  path = new OdePath(c1);
  rad = new OdeRadarSensor(obs);
  //_pins = new OdePathIns();
  ins->addSubscription(path->getPathIns());
  ins->addSubscription(rad->getPathIns());
  rad->addSubscription(path->getRadarSensor());

  if (DEBUG2)  {
    int cblue = 0x8EBAE5;
    printf("rgb %i,%i,%i\n", (cblue >> 16) & 255, (cblue >> 8) & 255,
	   cblue & 255);
  }


  // run simulation
  dsSimulationLoop (argc,argv,640,524,&fn);

  fclose(ofile);

  dJointGroupDestroy (contactgroup);
  dSpaceDestroy (space);
  dWorldDestroy (world);
  dCloseODE();
  return 0;
}

