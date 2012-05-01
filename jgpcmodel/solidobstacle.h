#ifndef OBST_H
#define OBST_H

#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include "texturepath.h"
#include <stdio.h>
#include "util.h"

#define MAXOBSTACLES 100

/* Obstacles is a class file to hold the obstacles that are in the world
   available for interaction

   typical call to create the object is:

   obs = new Obstacles();
   obs->loadFromFile("obstacles.dat");
   obs->setTypeColor(1,0xFFFF00);
   obs->setTypeColor(3,0x00C01E);
   obs->setTypeColor(6,0x00A600);
   obs->build(world, space, m);

   obstacle type codes
   1 box objects with body associated
   2 box objects with no body associated
   3 cylinder objects with body
   4 cylinder objects with no body
   5 sphere objects with body
   6 sphere objects with no body

   file format is tab delimited 7 fields N/A fields should be
   filled with zeroes.
   field names:
   type,length,width,height,density,px,py,pz,axis,rotation

 */


class Obstacles {
public:
  Obstacles() {init();};
  void init();
  void loadFromFile(const char *);
  void build(dWorldID, dSpaceID, dMass );
  int getSize() {return count;}
  void setColor(int, int); // set color of indiv object
  void setTypeColor(int, int); // change color for all objects of type
  void draw();
  P2D getOrigLocation(int);  // obs number
  P2D getCurrentLocation(int);  // obs number
  int getType(int);
  float getHeight(int);
  float getLength(int);
  float getWidth(int);
  const dReal *getCurrentPos(int i) {return dBodyGetPosition(bbod[i]);};
  dBodyID getBodyID(int);
protected:
  int type[MAXOBSTACLES];
  float length[MAXOBSTACLES];
  float width[MAXOBSTACLES];
  float height[MAXOBSTACLES];
  float density[MAXOBSTACLES];
  float px[MAXOBSTACLES];
  float py[MAXOBSTACLES];
  float pz[MAXOBSTACLES];
  dBodyID bbod[MAXOBSTACLES];
  dGeomID gid[MAXOBSTACLES];
  int axis[MAXOBSTACLES]; // axis of rotation
  float rotation[MAXOBSTACLES]; // rotation amount
  int color[MAXOBSTACLES];
  int count;  // number of obstacles in object
  
};

#endif
