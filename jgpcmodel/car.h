#ifndef car_h 
#define car_h

#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include "texturepath.h"
#include <stdio.h>
#include <math.h>
#include "util.h"

#define MAXBODYPARTS 3
#define FMAX 25
#define WMASS 0.2
#define MAXWAYPOINTS 10
#define MAXCOMMANDS 100 // number of commands allowed in cmd file

#define CCNGSTEPS 14 // value set to subcnt when changing course
#define CADJINTERVAL 30    // interval to reevaluate course and make adjustments

#define RECORDINTERVAL 10

#define CARDEBUG 1

/* Notes about the Car object*********
   The car object is typically called with something like these 2 examples:

   example 1: car running on waypoints with a simple start command that sets
   the original departure speed.  The car is equipped with radar which has
   been seeded with the current obstacle field data (must be called after
   the Obstacles object has been created

   c1 = new Car(LENGTH,WIDTH,HEIGHT,RADIUS);
   c1->setSpace(space);
   c1->makeCar(world,0.0, 0.0,STARTZ);
   c1->setBodyColor(0,0x800000);
   c1->loadCommandFile("test.csv");
   c1->loadWayPointFile("waypoints.dat");
   c1->toggleDrawWaypoints();
   c1->addRadar(4.0);
   c1->radar->connectObstacles(obs);
   if (c1->hasRadar)
    printf("Car has radar\n");

   a record of the track run by the car can be created by passing a valid 
   FILE pointer to the car recordTrack() method.  The format of the track
   file is comma separated counter,px,py,pz

   Second style of car called to follow a set of commands provided in a
   command file -- it does not have radar nor waypoints
   c2 = new Car(LENGTH*1.5,WIDTH*1.3,HEIGHT*1.1,RADIUS*1.2);
   c2->setSpace(space);
   c2->makeCar(world,1.0, 2.0,STARTZ);
   c2->setBodyColor(0,0x539DC2);   // carolina blue
   c2->loadCommandFile("cmd-1.cmd");


   commands from inside the simLoop function:

    c1->step(); or
    c1->step(speed,steer);
    c1->draw();
    

 */

// auto commands
typedef struct {
  int cnt;
  float spd;
  float dir;
} CMDS;


class Car  {
  public:
  Car() {init();};
  Car(float l, float w, float h, float r) {
    clength = l; 
    cwidth = w; 
    cheight = h; 
    wradius = r;
    subcnt = -1;
    counter = 0;
    speed =0.0;
    steer = 0.0;
    //showWaypoints = false;
    steering = false;
    ready = 0;
    out = NULL;};
  void makeCar(dWorldID, dReal , dReal, dReal);
  void makeCar(dWorldID, dReal, dReal, dReal, float, float, float, float);
  void draw();  // method to draw car in scene
  void step(float, float);  // step with speed & steer adjustment
  void step();   // normal simulation step
  void setsize(float , float , float , float );  // clength,cwidth,cheight,wradius
  void setSpace(dSpaceID);
  float getWheelRadius() {return(wradius);}
  void init(dSpaceID, float, float, float, float);
  void init();
  void loadCommandFile(const char*);
  void loadWayPointFile(const char*);
  const dReal *getCarPos() {return dBodyGetPosition(body[0]);};
  const dReal *getCarAttitude() {return dGeomGetRotation(box[0]);};
  // change speed return speed
  float adjustSpeed(float spd) {speed += spd; return speed;}; //modify speed
  float setSpeed(float spd) {speed = spd; return speed;}; // absolute speed set 
  // change speed return speed
  float getSpeed() {return speed;};
  float adjustCourse(float turn) {steer = turn; return steer;}; 
  void setBodyColor(int, int); // color of body part
  float getHeading();  // returns current heading
  //float getDestinationHeading(); // returns heading of vector from current pos to destination
  void gatherPositionAndAttitude(double *);
  float steer2heading(float); // steers toward parameter returns current heading
  void recordTrack(FILE *ofile) {out = ofile;};
  float distanceRemaining(P2D , P2D ); // from, to
 protected:
  float clength;  // chassis length
  float cwidth;  // chassis width
  float cheight; //  chassis height
  float cmass;   // chassis mass
  int bodyPartCount;
  int numCommands;
  int nextCommand;
  //int numWaypoints;
  //int wpcounter;
  int ready;
  float wradius; // wheel radius
  dBodyID body[MAXBODYPARTS+4];
  dJointID joint[4];       
  dJointGroupID contactgroup;
  dSpaceID space, carspace;
  dGeomID box[MAXBODYPARTS];
  dGeomID sphere[4];
  int color[MAXBODYPARTS];
  CMDS cmds[MAXCOMMANDS];
  //P2D waypoints[MAXWAYPOINTS];
  //P2D destination, currentpos, prevpos;
  int subcnt,counter;
  float speed, steer,adjsteer;
  //bool showWaypoints;
  bool steering;
  FILE *out;
  float YoffsetFromGoal(P2D,P2D,float);
};

#endif
