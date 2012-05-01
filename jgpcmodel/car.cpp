#include "car.h"


void Car::makeCar(dWorldID world, dReal x, dReal y, dReal z,float len, float wid, float ht, float wrad) {
  int i, bodyI = 0, boxI = 0, jointI = 0, sphI = 0;
  dMass m;

  if (!ready) {
    printf("Car not ready -- no space assigned yet\n");
    return;
  }

  setsize(len, wid, ht, wrad);
  // chassis body
  body[bodyI] = dBodyCreate (world);
  dBodySetPosition (body[bodyI],x,y,z);
  dMassSetBox (&m,1,clength,cwidth,cheight);
  dMassAdjust (&m,0.5);
  dBodySetMass (body[bodyI],&m);
  // make in space 0 then build together into carspace and assign it to space to make sim faster
  box[boxI] = dCreateBox (0,clength,cwidth,cheight);
  dGeomSetBody (box[boxI],body[bodyI]);
	
  // wheel bodies
  for (i=1; i<=4; i++) {
    body[bodyI+i] = dBodyCreate (world);
    dQuaternion q;
    dQFromAxisAndAngle (q,1,0,0,M_PI*0.5);
    dBodySetQuaternion (body[bodyI+i],q);
    dMassSetSphere (&m,1,wradius);
    dMassAdjust (&m,0.25);
    dBodySetMass (body[bodyI+i],&m);
    sphere[sphI+i-1] = dCreateSphere (0,wradius);
    dGeomSetBody (sphere[sphI+i-1],body[bodyI+i]);
  }
  // body 1-2 front wheels 3-4 back wheels
  //  push front wheels a little farther forward
  //dBodySetPosition (body[bodyI+1],x+0.4*clength-0.5*wradius,
  //                 y+cwidth*0.5,z-cheight*0.5);
  dBodySetPosition (body[bodyI+1],x+0.5*clength-0.25*wradius,
		    y+cwidth*0.5,z-cheight*0.5);
  dBodySetPosition (body[bodyI+2],x+0.5*clength-0.25*wradius,
		    y-cwidth*0.5,z-cheight*0.5);
  dBodySetPosition (body[bodyI+3],x-0.4*clength+0.5*wradius,
		    y+cwidth*0.5,z-cheight*0.5);
  dBodySetPosition (body[bodyI+4],x-0.4*clength+0.5*wradius,
		    y-cwidth*0.5,z-cheight*0.5);
	
  // front and back wheel hinges
  for (i=0; i<4; i++) {
    joint[jointI+i] = dJointCreateHinge2 (world,0);
    dJointAttach (joint[jointI+i],body[bodyI],body[bodyI+i+1]);
    const dReal *a = dBodyGetPosition (body[bodyI+i+1]);
    dJointSetHinge2Anchor (joint[jointI+i],a[0],a[1],a[2]);
    dJointSetHinge2Axis1 (joint[jointI+i],0,0,(i<2 ? 1 : -1));
    dJointSetHinge2Axis2 (joint[jointI+i],0,1,0);
    dJointSetHinge2Param (joint[jointI+i],dParamSuspensionERP,0.4);
    dJointSetHinge2Param (joint[jointI+i],dParamSuspensionCFM,0.8);
    if (i < 2) {
      dJointSetHinge2Param (joint[jointI+i],dParamVel2,0);
      dJointSetHinge2Param (joint[jointI+i],dParamFMax2,FMAX);
      dJointSetHinge2Param (joint[jointI+i],dParamStopERP,0.99f);
      dJointSetHinge2Param (joint[jointI+i],dParamStopCFM,0.001f);
    } 
    // lock wheels along the steering axis
    dJointSetHinge2Param (joint[i],dParamLoStop,0);
    dJointSetHinge2Param (joint[i],dParamHiStop,0);
    

  }

  // create a small black box on top
  body[bodyI+5] = dBodyCreate (world);
  dBodySetPosition (body[bodyI+5],x,y,z+cheight*0.6);
  dMassSetBox (&m,1,clength*0.5,cwidth*0.6,cheight*0.6);
  dMassAdjust (&m,0.5);
  dBodySetMass (body[bodyI+5],&m);
  dJointID j = dJointCreateFixed(world, 0);
  dJointAttach(j, body[bodyI], body[bodyI+5]);
  dJointSetFixed(j);
  box[boxI+1] = dCreateBox (0,clength*0.5,cwidth*0.6,cheight*0.6);
  dGeomSetBody (box[boxI+1],body[bodyI+5]);

  bodyPartCount = 2;

  carspace = dSimpleSpaceCreate(space);
  dSpaceSetCleanup(carspace,0);
  for (i = 0; i < bodyPartCount; i++) 
    dSpaceAdd(carspace,box[i]);
  for (i = 0; i < 4; i++)
    dSpaceAdd(carspace,sphere[i]);


  // set default colors for the 2 body part boxes
  setBodyColor(0,0xCC5500);  // orange
  setBodyColor(1,0xFFFFFF);
  /*
  currentpos.px = x;
  currentpos.py = y;
  prevpos.px = 0;
  prevpos.py = 0;
  */	
  //center of mass offset body. 
  //(hang another copy of the body COMOFFSET units below it by a fixed joint)
  /*
  dBodyID b = dBodyCreate (world);
  dBodySetPosition (b,x,y,z+COMOFFSET);
  dMassSetBox (&m,1,clength,cwidth,cheight);
  dMassAdjust (&m,CMASS/2.0);
  dBodySetMass (b,&m);
  dJointID j = dJointCreateFixed(world, 0);
  dJointAttach(j, body[bodyI], b);
  dJointSetFixed(j);
  //box[boxI+1] = dCreateBox(space,clength,cwidth,cheight);
  //dGeomSetBody (box[boxI+1],b);
  */	

};

void Car::makeCar(dWorldID world, dReal x, dReal y, dReal z) {
  int i, bodyI = 0, boxI = 0, jointI = 0, sphI = 0;
  dMass m;
  
   if (!ready) {
    printf("Car not ready -- no space assigned yet\n");
    return;
  }
 
  // chassis body
  body[bodyI] = dBodyCreate (world);
  dBodySetPosition (body[bodyI],x,y,z);
  dMassSetBox (&m,1,clength,cwidth,cheight);
  dMassAdjust (&m,0.5);
  dBodySetMass (body[bodyI],&m);
  // make in space 0 then build together into carspace and assign it to space to make sim faster
  box[boxI] = dCreateBox (0,clength,cwidth,cheight);
  dGeomSetBody (box[boxI],body[bodyI]);
	
  // wheel bodies
  for (i=1; i<=4; i++) {
    body[bodyI+i] = dBodyCreate (world);
    dQuaternion q;
    dQFromAxisAndAngle (q,1,0,0,M_PI*0.5);
    dBodySetQuaternion (body[bodyI+i],q);
    dMassSetSphere (&m,1,wradius);
    dMassAdjust (&m,WMASS);
    dBodySetMass (body[bodyI+i],&m);
    sphere[sphI+i-1] = dCreateSphere (0,wradius);
    dGeomSetBody (sphere[sphI+i-1],body[bodyI+i]);
  }
  // body 1-2 front wheels 3-4 back wheels
  //  push front wheels a little farther forward
  //dBodySetPosition (body[bodyI+1],x+0.4*clength-0.5*wradius,
  //                 y+cwidth*0.5,z-cheight*0.5);
  dBodySetPosition (body[bodyI+1],x+0.5*clength-0.25*wradius,
		    y+cwidth*0.5,z-cheight*0.5);
  dBodySetPosition (body[bodyI+2],x+0.5*clength-0.25*wradius,
		    y-cwidth*0.5,z-cheight*0.5);
  dBodySetPosition (body[bodyI+3],x-0.4*clength+0.5*wradius,
		    y+cwidth*0.5,z-cheight*0.5);
  dBodySetPosition (body[bodyI+4],x-0.4*clength+0.5*wradius,
		    y-cwidth*0.5,z-cheight*0.5);
	
  // front and back wheel hinges
  for (i=0; i<4; i++) {
    joint[jointI+i] = dJointCreateHinge2 (world,0);
    dJointAttach (joint[jointI+i],body[bodyI],body[bodyI+i+1]);
    const dReal *a = dBodyGetPosition (body[bodyI+i+1]);
    dJointSetHinge2Anchor (joint[jointI+i],a[0],a[1],a[2]);
    dJointSetHinge2Axis1 (joint[jointI+i],0,0,(i<2 ? 1 : -1));
    dJointSetHinge2Axis2 (joint[jointI+i],0,1,0);
    dJointSetHinge2Param (joint[jointI+i],dParamSuspensionERP,0.4);
    dJointSetHinge2Param (joint[jointI+i],dParamSuspensionCFM,0.8);
    if (i < 2) {
      dJointSetHinge2Param (joint[jointI+i],dParamVel2,0);
      dJointSetHinge2Param (joint[jointI+i],dParamFMax2,FMAX);
      dJointSetHinge2Param (joint[jointI+i],dParamStopERP,0.99f);
      dJointSetHinge2Param (joint[jointI+i],dParamStopCFM,0.001f);
    } 
    // lock wheels along the steering axis
    dJointSetHinge2Param (joint[i],dParamLoStop,0);
    dJointSetHinge2Param (joint[i],dParamHiStop,0);
    

  }

  // create a small black box on top
  body[bodyI+5] = dBodyCreate (world);
  dBodySetPosition (body[bodyI+5],x,y,z+cheight*0.6);
  dMassSetBox (&m,1,clength*0.5,cwidth*0.6,cheight*0.6);
  dMassAdjust (&m,0.5);
  dBodySetMass (body[bodyI+5],&m);
  dJointID j = dJointCreateFixed(world, 0);
  dJointAttach(j, body[bodyI], body[bodyI+5]);
  dJointSetFixed(j);
  box[boxI+1] = dCreateBox (0,clength*0.5,cwidth*0.6,cheight*0.6);
  dGeomSetBody (box[boxI+1],body[bodyI+5]);

  bodyPartCount = 2;


  carspace = dSimpleSpaceCreate(space);
  dSpaceSetCleanup(carspace,0);
  for (i = 0; i < bodyPartCount; i++) 
    dSpaceAdd(carspace,box[i]);
  for (i = 0; i < 4; i++)
    dSpaceAdd(carspace,sphere[i]);


  // set default colors for the 2 body part boxes
  setBodyColor(0,0xCC5500);  // orange
  setBodyColor(1,0xFFFFFF);
  /*
  currentpos.px = x;
  currentpos.py = y;
  prevpos.px = 0;
  prevpos.py = 0;
  */	
  //center of mass offset body. 
  //(hang another copy of the body COMOFFSET units below it by a fixed joint)
  /*
  dBodyID b = dBodyCreate (world);
  dBodySetPosition (b,x,y,z+COMOFFSET);
  dMassSetBox (&m,1,clength,cwidth,cheight);
  dMassAdjust (&m,CMASS/2.0);
  dBodySetMass (b,&m);
  dJointID j = dJointCreateFixed(world, 0);
  dJointAttach(j, body[bodyI], b);
  dJointSetFixed(j);
  //box[boxI+1] = dCreateBox(space,clength,cwidth,cheight);
  //dGeomSetBody (box[boxI+1],b);
  */	

};


void Car::init(dSpaceID sp, float clen, float cwid, float cht, float rad) {

  setSpace(sp);
  setsize(clen, cwid, cht, rad);
  subcnt = -1;
  counter = 0;
  speed =0.0;
  steer = 0.0;
  //showWaypoints = false;
  steering = false;
  ready = 1;
  out = NULL;
 };


void Car::init() {
  // default values;
  setsize(0.7, 0.5, 0.2, 0.18);
  cmass = 1.0;
  subcnt = -1;
  counter = 0;
  speed =0.0;
  steer = 0.0;
  //showWaypoints = false;
  steering = false;
  out = NULL;
  ready = 0;
};


void Car::draw() {
  if (!ready)
    return;
  float r,g,b;
  int i;
  b = (color[0] & 255) / 255.0;
  g = ((color[0] >> 8) & 255) / 255.0;
  r = ((color[0] >> 16) & 255) /255.0;
  dsSetColor(r,g,b);
  //dsSetColor (0,0.65,0);

  dReal sides[3] = {clength,cwidth,cheight};
  dsDrawBox (dGeomGetPosition(box[0]),dGeomGetRotation(box[0]),sides);
  b = (color[1] & 255) / 255.0;
  g = ((color[1] >> 8) & 255) / 255.0;
  r = ((color[1] >> 16) & 255) /255.0;
  dsSetColor(r,g,b);
  dVector3 smsides;
  dGeomBoxGetLengths (box[1],smsides);
  dsDrawBox (dGeomGetPosition(box[1]),dGeomGetRotation(box[1]),smsides);
  // set wheels to black
  dsSetColor (1,1,1);
  for (i=0; i< 4; i++) dsDrawCylinder (dGeomGetPosition(sphere[i]),
					   dGeomGetRotation(sphere[i]),
					     0.02f,wradius);

};

void Car::step(float spd, float dir) {
  if (!ready)
    return;

  speed += spd;
  steer += dir;

  step();


};

void Car::step() {
  const dReal *a = getCarPos();
  /* 
  if (subcnt-- == 0) {
    steer -= adjsteer;
    adjsteer = 0.0;
    prevpos.px = a[0];
    prevpos.py = a[1];
  }
  */
  if (out != NULL && counter % RECORDINTERVAL == 0) 
    fprintf(out,"%i,%f,%f,%f\n",counter,a[0],a[1],a[2]);


  if (counter == cmds[nextCommand].cnt) {
    speed += cmds[nextCommand].spd;
    steer += cmds[nextCommand].dir;
    //printf("Counter %i speed %f steer %f\n",cmds[nextCommand].cnt,speed,steer);
    // increment and protect against overflow
    nextCommand++;
    if (nextCommand == numCommands)
      nextCommand = 0;
  }
  
  //motor
  dJointSetHinge2Param (joint[0],dParamVel2,-speed);
  dJointSetHinge2Param (joint[0],dParamFMax2,0.1);
  dJointSetHinge2Param (joint[1],dParamVel2,-speed);
  dJointSetHinge2Param (joint[1],dParamFMax2,0.1);
  //steering
  dReal v = steer - dJointGetHinge2Angle1 (joint[0]);
  if (v > 0.1) v = 0.1;
  if (v < -0.1) v = -0.1;
  v *= 10.0;
  dJointSetHinge2Param (joint[0],dParamVel,v);
  dJointSetHinge2Param (joint[0],dParamFMax,0.2);
  dJointSetHinge2Param (joint[0],dParamLoStop,-0.75);
  dJointSetHinge2Param (joint[0],dParamHiStop,0.75);
  dJointSetHinge2Param (joint[0],dParamFudgeFactor,0.1);
  dJointSetHinge2Param (joint[1],dParamVel,v);
  dJointSetHinge2Param (joint[1],dParamFMax,0.2);
  dJointSetHinge2Param (joint[1],dParamLoStop,-0.75);
  dJointSetHinge2Param (joint[1],dParamHiStop,0.75);
  dJointSetHinge2Param (joint[1],dParamFudgeFactor,0.1);

  counter++;

};

void Car::setsize(float clen, float cwid, float cht, float rad) {
  clength = clen;
  clength = cwid;
  cheight = cht;
  wradius = rad;
};

void Car::setSpace(dSpaceID sp) {
  space = sp;
  ready = 1;
};

void Car::loadCommandFile(const char* infile) {
  FILE *in = fopen(infile,"r");
  if (in == NULL) {
    printf("cannot open %s\n",infile);
    numCommands = 0;
    nextCommand = -1;
    return;
  }
  int r = 0;
  while (!feof(in)) {
    fscanf(in,"%i\t%f\t%f\n",&cmds[r].cnt,&cmds[r].spd,&cmds[r].dir);
    if (r < MAXCOMMANDS-1)
      r++;
  }
  numCommands = r;
  nextCommand = 0;
  fclose(in);
};
void Car::setBodyColor(int bpart, int shade) {
  if (bpart < bodyPartCount) 
    color[bpart] = shade;
}


float Car::getHeading() {
  /* test to look at the y axis rotation to test for heading
     the values returned in y[4] and y[5] can be used to determine
     the current heading using the following formula (tangent of angle
     of interest equals the opposite side over adjacent side):
     tan(q) = y[4] / y[5]
     q = arctan(y[4] / y[5]) ; q is expressed in radians
     heading = 360 - (q * 57.29578)
     heading has potential for same kind of issues that the dheading has
     heading from the y4 and y5 values only give the angle within a quadrant,
     the signs of the two values must be used to determine the proper quandrant,
     and hence the constant degrees to remove the value from.  When y[4] approaches
     1 and y[5] approaches 0 the angle is approaching 90 degrees (subtracted from 360
     equals 270, which is looking directly down the y axis from the original perspective).
     
     +y[4] && +y[5]        |         -y[4] && +y[5]
     ______________________|_______________________
                           |
     +y[4] && -y[5]        |         -y[4] && -y[5]
     
     values in 4 quadrants (counter clockwise)
     1  360 - 270
     2  270 - 180
     3  180 - 90
     4  90 - 0 
     
  */
  float heading;
  const dReal *y = dGeomGetRotation(box[0]);
  // determine quadrant and do calc
  if (y[4] > 0 && y[5] > 0) { // q1
    heading = 360 -  (atanf(y[4]/y[5])*57.29578);
  } else if (y[4] > 0 && y[5] < 0) { //q2
    heading = 270 - (90 +  (atanf(y[4]/y[5])*57.29578));
  } else if (y[4] < 0 && y[5] < 0) { // q3
    heading = 180 -  (atanf(y[4]/y[5])*57.29578);
  } else { // q4
    //heading = fabsf(atanf( (atanf(y[4]/y[5])*57.29578))); // note the double atanf error!!!
    heading =  fabsf(atanf(y[4]/y[5])*57.29578);
  }
  return(heading);

};
/*
float Car::getDestinationHeading() {
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
*/
void Car::gatherPositionAndAttitude(double *pos) {
  // pos is a 6 value array of doubles roll, pitch, yaw, x, y, z
  // calc these later
  pos[0] = 0.0;
  pos[1] = 0.0;
  pos[2] = (double) getHeading();
  const dReal *p = getCarPos();
  pos[3] = (double) p[0];
  pos[4] = (double) p[1];
  pos[5] = (double) p[2];
};

float Car::YoffsetFromGoal(P2D p2, P2D p1, float gx) {
  /* the formula for the line that runs thru 2 points is:
              (y2 - y1)
     y - y1 = --------- (x - x1)
              (x2 -x1)

     this function uses that formula to cast the the line of current direction
     through the y axis of the destination (using the x location of the dest in
     gx) and returns the y location along the destination x where the current
     line would intersect
  */
  float x,y,m;
  m = (p2.py - p1.py) / (p2.px - p1.px);
  return(m*(gx - p1.px) + p1.py);
}

float Car::distanceRemaining(P2D from, P2D to) {
  float dx, dy;
  dx = from.px - to.px;
  dy = from.py - to.py;
  return(sqrt(pow(dx,2) + pow(dy,2)));
};


float steer2heading(float goal) {

  return 0.0;
};
