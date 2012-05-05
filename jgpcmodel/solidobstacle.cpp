
#include "solidobstacle.h"

void Obstacles::init() {
  count = 0;
}

void Obstacles::loadFromFile(const char *fname) {
  FILE *ofile;
  int r = 0;
  ofile = fopen(fname,"r");
  if (ofile == NULL) {
    printf("cannot open %s -- setting obstacles to 0\n",fname);
    count = 0;
    //exit(-1);
  } else {
    while (!feof(ofile) && r < MAXOBSTACLES) {
      fscanf(ofile,"%i\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%i\t%f\n",
	     &type[r],&length[r],&width[r],
	     &height[r],&density[r],&px[r],&py[r],&pz[r],
	     &axis[r],&rotation[r]);
      // set default color to black
      color[r] = 0xFFFFFF;
      /*
      if (DEBUG)
	printf("%i\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n",
	       obs[r].type,obs[r].length,obs[r].width,obs[r].height,
	       obs[r].density,obs[r].px,obs[r].py,obs[r].pz);
      */
      if (r < MAXOBSTACLES)
	r++;
    }
    count = r;
    fclose(ofile);
  }
};
/* obstacle type codes
   1 box objects with body associated
   2 box objects with no body associated
   3 cylinder objects with body
   4 cylinder objects with no body
   5 sphere objects with body
   6 sphere objects with no body
 */

void Obstacles::build(dWorldID world, dSpaceID space, dMass m) {
  dMatrix3 R;
  for (int i = 0; i < count; i++) {
    if (type[i] % 2 == 1) { // odd number types have a body associated with them
      bbod[i] = dBodyCreate(world);
      dMassSetBox (&m,density[i],
		   length[i],width[i],height[i]);
      dBodySetMass (bbod[i],&m);
    }
    switch(type[i]) {
    case 1:
      gid[i] = dCreateBox(space,length[i],width[i],height[i]);
      dGeomSetBody(gid[i], bbod[i]);
      break;
    case 2:
      gid[i] = dCreateBox(space,length[i],width[i],height[i]);
      break;
    case 3:
      gid[i] = dCreateCylinder(space,width[i],height[i]); // width = radius for cyl height is length
      dGeomSetBody(gid[i], bbod[i]);
      break;
    case 4:
      gid[i] = dCreateCylinder(space,width[i],height[i]); // width = radius for cyl height is length
      break;
    case 5:
      gid[i] = dCreateSphere(space,width[i]); // width = radius for sphere
      dGeomSetBody(gid[i],bbod[i]);
      break;
    case 6:
      gid[i] = dCreateSphere(space,width[i]); // width = radius for sphere
      break;
    }
    dGeomSetPosition (gid[i],px[i],py[i],pz[i]);
    
    if (axis[i] == 1) {
      dRFromAxisAndAngle (R,1,0,0,rotation[i]);
      dGeomSetRotation (gid[i],R);
    } else if (axis[i] == 2) {
      dRFromAxisAndAngle (R,0,1,0,rotation[i]);
      dGeomSetRotation (gid[i],R);
    } else if (axis[i] == 3) {
      dRFromAxisAndAngle (R,0,0,1,rotation[i]);
      dGeomSetRotation (gid[i],R);
    }
    
      
  }


};

void Obstacles::setColor(int obj, int shade) {
  if (obj < count && obj >= 0) {
    color[obj] = shade;
  }
};

void Obstacles::setTypeColor(int typ, int shade) {
  for (int i = 0; i < count; i++) 
    if (type[i] == typ)
      setColor(i, shade);
}

void Obstacles::draw() {
  dVector3 ss;
  float r,g,b;
  for (int i = 0; i < count; i++) {
    b = (color[i] & 255) / 255.0;
    g = ((color[i] >> 8) & 255) / 255.0;
    r = ((color[i] >> 16) & 255) /255.0;
    dsSetColor(r,g,b);
    switch(type[i]) {
    case 1:
    case 2:
      dGeomBoxGetLengths (gid[i],ss);
      dsDrawBoxD (dGeomGetPosition(gid[i]),dGeomGetRotation(gid[i]),ss);
      break;
    case 3:
    case 4:
      dsDrawCylinderD(dGeomGetPosition(gid[i]),dGeomGetRotation(gid[i]),height[i],width[i]);
      break;
    case 5:
    case 6:
      dsDrawSphereD(dGeomGetPosition(gid[i]),dGeomGetRotation(gid[i]),2.0);
    }

  }
};

P2D Obstacles::getOrigLocation(int i) {
  P2D loc;
  if (i > count-1) {
    loc.px = 0.0;
    loc.py = 0.0;
    printf("Error: Obstacle location requested for non existent obstacle\n");
    return loc;
  }

  loc.px = px[i];
  loc.py = py[i];

  return loc;
};

P2D Obstacles::getCurrentLocation(int i) {
  P2D loc;
  if (i > count-1) {
    loc.px = 0.0;
    loc.py = 0.0;
    printf("Error: Obstacle location requested for non existent obstacle\n");
    return loc;
  }

  if (type[i] % 2 == 1) { // type with a body attached
    const dReal *a = getCurrentPos(i);
    loc.px = a[0];
    loc.py = a[1];
  } else {
    loc.px = px[i];
    loc.py = py[i];
  }
  return loc;
};

int Obstacles::getType(int i) {
  if (i > count-1) {
    printf("Error: Obstacle info requested for non existent obstacle (%i)\n",i);
    return -1;
  }

  return type[i];
};


float Obstacles::getHeight(int i) {
  if (i > count-1) {
    printf("Error: Obstacle info requested for non existent obstacle (%i)\n",i);
    return -1.0;
  }

  return height[i];
};

float Obstacles::getLength(int i) {
  if (i > count-1) {
    printf("Error: Obstacle info requested for non existent obstacle (%i)\n",i);
    return -1.0;
  }

  return length[i];
};

float Obstacles::getWidth(int i) {
  if (i > count-1) {
    printf("Error: Obstacle info requested for non existent obstacle (%i)\n",i);
    return -1.0;
  }

  return width[i];
};


dBodyID Obstacles::getBodyID(int i) {
  if (i > count-1) {
    printf("Error: Obstacle info requested for non existent obstacle (%i)\n",i);
    return NULL;
  }

  return bbod[i];
};
