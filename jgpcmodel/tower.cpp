#include <ode/ode.h>
#include <drawstuff/drawstuff.h>
#include "texturepath.h"
#include <stdio.h>
#include "tower.h"

void Tower::build() {
  tower = dCreateCylinder(space, tbase, theight);
  dGeomSetPosition(tower,tx, ty, 0.0);
};

void Tower::draw() {
  dsSetColor(1,1,1);
  dsDrawCylinder(dGeomGetPosition(tower), dGeomGetRotation(tower),theight,
		 tbase);
  counter++;
};
