
#include "radar.h"


int Radar::scan(BOGEY *hits, P2D pos, float heading) {
  int i, j;
  P2D verts[SCANLINES+1];
  P2D lines[4];
  int hitcnt = 0;
  setDir(heading);
  float scanLeft = heading - (arc / 2.0);
  float scanRight = heading + (arc / 2.0);
  bool foundHit = false;

  if (ocnt < 1) {
    printf("Error: called radar object without assiging obstacle object\n");
    bogeyhits = 0;
    return -1;
  }  
  /* build a polygon from pos -> point at scanLeft length away -> along arc from scanL to scanR ->
     then back to pos
  */
  verts[0] = pos;
  lines[0] = pos;
  scancnt = 1;
  scanning = true;
  for (i = 1; i < SCANLINES; i++) {
    verts[i] = arcPoint(scanRight-(i*(arc/SCANLINES)),pos.px,pos.py,length);
  }
  /* // jPoly version of scanning
  verts[SCANLINES] = pos;
  // test each obstacle point to see if it is in the polygon
  jPoly *target = new jPoly(SCANLINES+1,verts);
  currentScan->numPoints = target->numPoints;
  currentScan->bbox = target->bbox;
  currentScan->valid = target->valid;
  for (i = 0; i < currentScan->numPoints; i++) {
    currentScan->points[i].px = target->points[i].px;
    currentScan->points[i].py = target->points[i].py;
  }
  */
  // we only get here if we have an obstacle object
  P2D tpoint;
  P2D cpoints[7];
  if (RADARDEBUG > 1) {
    printf("RDBG: heading %f \n (%f,%f,%f,%f,%f,%f,\n%f,%f,%f,%f,%f,%f)\n",dir,
	   verts[0].px,verts[0].py,verts[1].px,verts[1].py,verts[2].px,verts[2].py,
	   verts[3].px,verts[3].py,verts[4].px,verts[4].py,verts[5].px,verts[5].py);
  }
  for (i = 0; i < ocnt; i++) {
    // this method gets the current position for a movable obstacle getOrigLocation gets the
    // original location
    tpoint = sobs->getCurrentLocation(i);
    foundHit = false;
    if (RADARDEBUG > 2) 
      printf("RDBG: tpoint %f,%f\n",tpoint.px,tpoint.py);
    // test first to see if it is within range of the radar or the right direction
    if (distanceBetween(verts[0],tpoint) < getLength() + 0.4) {
      // in range test that direction is inside arc
      if (headingBetween(scanLeft,scanRight,getHeadingToPoint(verts[0],tpoint))) {
	// within the arc -- test for intersection with the arcs
	// create the lines for the object
	/* for a circle you only need to test whether the point 180 degrees from the direction
	   of the radar line is on the line */
	// **** not done yet build a set of lines to depict shape of object lines[2] and lines[3]
	for (j = 1; j < SCANLINES && !foundHit; j++) {
	  lines[1] = verts[j];
	  int type = sobs->getType(i);
	  if (type == 3 || type == 4) { //cylinder
	    circleTestPoints(heading,sobs->getWidth(i),tpoint.px,tpoint.py,cpoints);
	    // test the 6 lines in cpoints
	    for (int k = 0; k < 7; k++) {
	      if (k > 0) {
		lines[2] = lines[3];
		lines[3] = cpoints[k];
	      } else {
		lines[2] = cpoints[k];
		lines[3] = cpoints[k+1];
	      }
	      //printf("ctest: lines[3].px, .py %f %f\n",lines[3].px,lines[3].py);
	      if (segmentsIntersect(lines))
		foundHit = true;
	    }
	  } else  { 
	    // test each line for intersection pass array with the 4 end points of the 2 lines
	    // bottom
	    lines[3].px = tpoint.px + (sobs->getWidth(i) / 2);
	    lines[2].px = tpoint.px - (sobs->getWidth(i) / 2);
	    lines[3].py = tpoint.py - (sobs->getLength(i) / 2);
	    lines[2].py = tpoint.py - (sobs->getLength(i) / 2);
	    if (segmentsIntersect(lines))
		foundHit = true;
	    // top 
	    lines[3].py = tpoint.py + (sobs->getLength(i) / 2);
	    lines[2].py = tpoint.py + (sobs->getLength(i) / 2);
	    if (segmentsIntersect(lines))
		foundHit = true;
	    // left
	    lines[3].px = lines[2].px;
	    lines[3].py = tpoint.py - (sobs->getLength(i) / 2);
	    if (segmentsIntersect(lines))
		foundHit = true;
	    // right
	    lines[3].px = tpoint.px + (sobs->getWidth(i) / 2);
	    lines[2].px = tpoint.px + (sobs->getWidth(i) / 2);
	    if (segmentsIntersect(lines))
		foundHit = true;
	    
	    //intersection point is located in lines[1]
	  }
	  if (foundHit) {
	    hits[hitcnt].obsType = sobs->getType(i);
	    hits[hitcnt].px = lines[1].px;
	    hits[hitcnt].py = lines[1].py;
	    hits[hitcnt].pheight = sobs->getHeight(i);
	    hits[hitcnt].identifier = i;  // cheating way to id the obs by its pos in obs file
	    hits[hitcnt].plength = sobs->getLength(i);
	    hits[hitcnt].pwidth = sobs->getWidth(i);
	    hits[hitcnt].reliability = 5.0; // radar sighting
	    hitcnt++;
	  }
	}
      }
    }  
  }
  bogeyhits = hitcnt;
  return hitcnt;
};


void Radar::draw(float height) {
  // draw five lines to represent the scan in 1 pass
  if (!scanning)
    return;
  // turn off the scanning
  scancnt = -1;
  scanning = false;
  int st = 0;
  float px = currentScan->points[st].px;
  float py = currentScan->points[st].py;
  const float p1[3] = {px,py,height};
  px = currentScan->points[st+1].px;
  py = currentScan->points[st+1].py;
  const float p2[3] = {px,py,0.5};
  px = currentScan->points[st+2].px;
  py = currentScan->points[st+2].py;
  const float p3[3] = {px,py,0.5};
  px = currentScan->points[st+3].px;
  py = currentScan->points[st+3].py;
  const float p4[3] = {px,py,0.5};
  px = currentScan->points[st+4].px;
  py = currentScan->points[st+4].py;
  const float p5[3] = {px,py,0.5};
  px = currentScan->points[st+5].px;
  py = currentScan->points[st+5].py;
  const float p6[3] = {px,py,0.5};
  if (RADARDEBUG > 1) 
    printf("p1:%f,%f p2:%f,%f p3:%f,%f p4:%f,%f p5:%f,%f p6:%f,%f\n",p1[0],p1[1],
	   p2[0],p2[1],p3[0],p3[1],p4[0],p4[1],p5[0],p5[1],p6[0],p6[1]);
  dsSetColor(1,0,0);
  dsDrawLine(p1,p2);
  dsDrawLine(p1,p3);
  dsDrawLine(p1,p4);
  dsDrawLine(p1,p5);
  dsDrawLine(p1,p6);
  dsSetColor(1,1,1);
  scancnt++;
};

P2D Radar::arcPoint(float deg, float x, float y, float r) {
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

bool Radar::segmentsIntersect(P2D *vtx) {
  double a, b;
  bool bx = false;
  bool by = false;

  a = vtx[0].px > vtx[1].px ? vtx[1].px : vtx[0].px;
  b = a == vtx[0].px ? vtx[1].px : vtx[0].px;

  if ((vtx[2].px >= a && vtx[2].px <= b) ||  (vtx[3].px >= a && vtx[3].px <= b))
    bx = true;

  a = vtx[0].py > vtx[1].py ? vtx[1].py : vtx[0].py;
  b = a == vtx[0].py ? vtx[1].py : vtx[0].py;

  if ((vtx[2].py >= a && vtx[2].py <= b) ||  (vtx[3].py >= a && vtx[3].py <= b))
    by = true;

  if (bx && by) { // segments intersect solve for intersection and return in vtx[0]
    double A1 = vtx[1].py-vtx[0].py;
    double B1 = vtx[0].px-vtx[1].px;
    double C1 = A1 * vtx[0].px + B1 * vtx[0].py;
    double A2 = vtx[3].py-vtx[2].py;
    double B2 = vtx[2].px-vtx[3].px;
    double C2 = A2 * vtx[2].px + B2 * vtx[2].py;
    double det = A1*B2 - A2*B1;
    if (fabs(det) < 0.001) { // approx zero lines are parallel
    } else {
      a = (B2*C1 - B1*C2) / det;
      b = (A1*C2 - A2*C1) / det;
    }
    // the line intersect but we need to make sure that the point of intersection
    // is on both lines
    bool ltest = true;
    for (int i = 0; i < 4; i+= 2) {
      // test the points on the segment
      if (min(vtx[i].px,vtx[i+1].px) > a  || max(vtx[i].px,vtx[i+1].px) < a)
	ltest = false;
      if (min(vtx[i].py,vtx[i+1].py) > b || max(vtx[i].py,vtx[i+1].py) < b)
	ltest = false;
    }
    if (ltest) {
      // put results back into vtx[1] since vtx[0] holds current position
      vtx[1].px = a;
      vtx[1].py = b;
      /*
      printf("Radar::segmentsIntersect: vtx[0-3] %f,%f:%f,%f:%f,%f:%f,%f\n",
	     vtx[0].px,vtx[0].py,vtx[1].px,vtx[1].py,
	     vtx[2].px,vtx[2].py,vtx[3].px,vtx[3].py);
      */
    } else
      bx = false;
  }

  return(bx && by);
};
float Radar::getHeadingToPoint(P2D currentpos, P2D destination) {
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


float Radar::distanceBetween(P2D from, P2D to) {
  float dx, dy;
  dx = from.px - to.px;
  dy = from.py - to.py;
  return(sqrt(pow(dx,2) + pow(dy,2)));
};

bool Radar::headingBetween(float left, float right, float dir) {
  bool ans;
  // need to work around the 360 = 0 problem
  if (left > right) {
    right += 360;
    if (dir < 30)
      dir += 360;
  }
  if (dir >= left && dir <= right)
    ans = true;
  else
    ans = false;
};

void Radar::circleTestPoints(float heading, float rad, float cx, float cy, P2D *cpoints) {
  /* start with a ray that is opposite of the heading located in the
     the center of the circle with radius of rad, and find and return the 3 points on either
     side 30 degrees apart.  Order of returned records is from outside point - 90 deg from
     ray to outside point + 90 degrees from ray
   */
  float rhead = heading <= 180.0 ? heading + 180.0 : heading - 180.0;
  float phead;
  int pcounter = 0;
  for (int i = -90; i < 91; i+=30) {
    phead = rhead + i;
    if (phead < 0.0)
      phead += 360.0;
    else if (phead > 360.0)
      phead -= 360.0;
    cpoints[pcounter++] = arcPoint(phead,cx,cy,rad);
  }
};

