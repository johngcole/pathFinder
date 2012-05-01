#ifndef tower_h 
#define tower_h

#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include "texturepath.h"
#include <stdio.h>
#include "util.h"
#include "worldmap.h"

#define MAXCONNECTIONS  4
#define MAXBOGIES  50


typedef struct {
  int connID;
  float px; // last reported postion of connector
  float py;
  int connectCounter;
  int lastConnect;
} CONNECT;
  

class Tower {
 public:
  Tower(dSpaceID space, float x, float y, float base, float height) {
    tx = x;
    ty = y;
    tbase = base;
    theight = height; 
    counter = 0;
    build();};
  void draw();
  CONNECT  makeConnection(float, float ); // requestor px, py
  Worldmap * queryTower(float, float); // requestor px, py
  void queryConnector(CONNECT *, Worldmap *);
 protected:
  dSpaceID space;
  dGeomID tower;
  float tx;
  float ty;
  float tbase;
  float theight;
  CONNECT connections[MAXCONNECTIONS];
  Worldmap map;
  int counter;
  //int bogeyCnt;
  int connCnt;
  void build();
};

#endif
