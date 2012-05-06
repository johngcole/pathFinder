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
*/


#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include "texturepath.h"
#include <stdio.h>
#include <math.h>

#include "solidobstacle.h"
#include "car.h"
//#include "tower.h"
#include "jPoly.h"
#include "util.h"
//#include "radar.h"
#include "OdePath.h"
#include "OdeInsSensor.h"
#include "OdeRadarSensor.h"

// some constants

#define LENGTH 0.7	// chassis length
#define WIDTH 0.5	// chassis width
#define HEIGHT 0.2	// chassis height
#define RADIUS 0.18	// wheel radius


// starting height of chassis when dropped at beginning of simulation 
//  original value = 0.5
#define STARTZ 0.5	// starting height of chassis



#define DEBUG 0
#define DEBUG2 0

// permanent structures to build

// dynamics and collision objects (chassis, 3 wheels, environment)

static dWorldID world;
static dSpaceID space;
static dJointGroupID contactgroup;
static dGeomID ground;
static dBodyID b;




int subcnt = -1;

int counter = 1;

Obstacles *obs;
Car *c1;
Car *car2follow = NULL;

OdeInsSensor *ins;
OdeRadarSensor *rad;
OdePath *path;

// things that the user controls

//static dReal speed=0,steer=0;	// user commands



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
  int class1,class2;
  // we want all collisions
  /*
  // only collide things with the ground
  int g1 = (o1 == ground || o1 == ground_box || o1 == bump || o1 == barrel);
  int g2 = (o2 == ground || o2 == ground_box || o2 == bump || o2 == barrel);
  if (!(g1 ^ g2)) return;
  */

  dBodyID b1 = dGeomGetBody(o1);
  dBodyID b2 = dGeomGetBody(o2);
  if (b1 && b2 && dAreConnected(b1, b2))
    return;

  // test for future implementation of ray class
  // (ray is class 5 box is 1 space is 10)
  /*
  class1 = dGeomGetClass(o1);
  class2 = dGeomGetClass(o2);
  if (class1 == 5 || class2  == 5) {
    if (class1 != 10 && class2 != 10) {
      n = dCollide(o1,o2,N,&contact[0].geom,sizeof(dContact));
      dContactGeom& cgArr = contact[0].geom;
      //printf("Ray collision detected at %i o1 %i o2 %i numContacts %i\n",counter,c1,c2,n);
      if (n > 0) {
	printf("geom pos %f,%f,%f\n",cgArr.pos[0],cgArr.pos[1],cgArr.pos[2]);
	printf("geom depth = %f\n",cgArr.depth);
      }
    }
    return;
  }
  */
  // test for space collisions (cars are defined as spaces) test is more efficient
  /*
  if (dGeomIsSpace (o1) || dGeomIsSpace (o2)) { 
    // colliding a space with something :
    dSpaceCollide2 (o1, o2, data,&nearCallback); 
 
    // collide all geoms internal to the space(s)
    if (dGeomIsSpace (o1))
      dSpaceCollide ((dSpaceID)o1, data, &nearCallback);
    if (dGeomIsSpace (o2))
      dSpaceCollide ((dSpaceID)o2, data, &nearCallback);
    
      } else {*/
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

	/* // orig values
	   contact[i].surface.mu = dInfinity;
	   contact[i].surface.slip1 = 0.1;
      contact[i].surface.slip2 = 0.1;
      contact[i].surface.soft_erp = 0.5;
      contact[i].surface.soft_cfm = 0.3;
	*/
	dJointID c = dJointCreateContact (world,contactgroup,&contact[i]);
	//dJointID c = dJointCreateContact (world,contactgroup,contact+i);
	dJointAttach (c,
		      dGeomGetBody(contact[i].geom.g1),
		      dGeomGetBody(contact[i].geom.g2));
      }
      //}
  }

}


// start simulation - set viewpoint

static void start()
{
  dAllocateODEDataForThread(dAllocateMaskAll);

  //static float xyz[3] = {0.8317f,-0.9817f,0.8000f}; // orig
  //static float hpr[3] = {121.0000f,-27.5000f,0.0000f}; //orig
  static float xyz[3] = {1.317f,-1.9817f,0.8000f};
  static float hpr[3] = {100.0000f,-27.5000f,0.0000f};
  dsSetViewpoint (xyz,hpr);
  /*
    printf ("Press:\t'a' to increase speed.\n"
	  "\t'z' to decrease speed.\n"
	  "\t',' to steer left.\n"
	  "\t'.' to steer right.\n"
	  "\t' ' to reset speed and steering.\n"
	  "\t'1' to save the current state to 'state.dif'.\n");
  // print the waypoint info
  
  if (numWaypoints > 0)
    printf("waypoint change wp %i %f %f cp %f %f\n",wpcounter,
	   waypoints[wpcounter].px,waypoints[wpcounter].py,
	   currentpos.px,currentpos.py);
  */
}


// called when a key pressed

static void command (int cmd)
{
  /*
  switch (cmd) {
  case 'a': case 'A':
    speed += 0.3;
    break;
  case 'z': case 'Z':
    speed -= 0.3;
    break;
  case ',':
    steer -= 0.5;
    break;
  case '.':
    steer += 0.5;
    break;
  case 'c':
    printf("current counter = %i\n",counter);
    break;
  case ' ':
    speed = 0;
    steer = 0;
    break;
  case '1': {
      FILE *f = fopen ("state.dif","wt");
      if (f) {
        dWorldExportDIF (world,f,"");
        fclose (f);
      }
    }
  }
  */
}


// simulation loop

static void simLoop (int pause)
{
  int i;
  /*
  // adjust viewpoint after 100 iterations and increase speed by 1.3
  if (counter == 100) {
    static float xyz[3] = {1.347f,-2.0213f,2.8800f};
    static float hpr[3] = {44.0000f,-29.0000f,0.0000f};
    dsSetViewpoint (xyz,hpr);
    //speed += 1.3;
  } 
  */

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
    //c2->step(speed,steer);
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
  
  dVector3 ss;
  

  
  obs->draw();

  dsSetColor (1,1,1);


  counter++;

}


int main (int argc, char **argv)
{
  FILE *in, *ofile, *wpfile;
  int i;
  int r = 0;
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
  
  /*
  c2 = new Car(LENGTH*1.5,WIDTH*1.3,HEIGHT*1.1,RADIUS*1.2);
  c2->setSpace(space);
  c2->makeCar(world,1.0, 2.0,STARTZ);
  c2->setBodyColor(0,0x539DC2);   // carolina blue
  c2->loadCommandFile("cmd-1.cmd");
  c2->addRadar(4.5);
  c2->radar->connectObstacles(obs);
  if (c2->hasRadar)
    printf("Car has radar\n");
  */
  car2follow = c1;

  ins = new OdeInsSensor(c1);
  path = new OdePath(c1);
  rad = new OdeRadarSensor(obs);
  ins->addSubscription(path->getPathIns());
  ins->addSubscription(rad->getPathIns());
  rad->addSubscription(path->getRadarSensor());

  if (DEBUG2)  {
    int cblue = 0x8EBAE5;
    printf("rgb %i,%i,%i\n", (cblue >> 16) & 255, (cblue >> 8) & 255,
	   cblue & 255);
  }



  /*
  // independent radar station
  rad = new Radar(4.0);
  BOGEY ht;
  P2D pt = {2.0,7.0};
  rad->connectObstacles(obs);
  for (i = 360; i > 0; i -= 30) {
    rad->scan(&ht,pt,(float) i);
    printf("Scan at %i found %i bogeys\n",i,rad->getHitCount());
  }
  */

  // run simulation
  //dsSimulationLoop (argc,argv,352,288,&fn);
  dsSimulationLoop (argc,argv,640,524,&fn);

  //printf("SimLoop ended -- control returned to main\n");
  fclose(ofile);

  dJointGroupDestroy (contactgroup);
  dSpaceDestroy (space);
  dWorldDestroy (world);
  dCloseODE();
  return 0;
}

