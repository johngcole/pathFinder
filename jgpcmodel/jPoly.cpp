#include "jPoly.h"
jPoly::jPoly(int len, P2D in[])  {
    int i;
    int x = len;
    // need at least 3 points to connect (since first and last are same takes arr size 4)
    if (x < 4) {
	valid = false;
	return;
    }
    // initialize a bounding box at point 0 (l,b,r,t)
    bbox.left = in[0].px;
    bbox.bottom = in[0].py;
    bbox.right = in[0].px;
    bbox.top = in[0].py;
    for (i = 0; i < x; i++) {
      points[i].px = in[i].px; 
      points[i].py = in[i].py;

      // update bbox
      if (points[i].py < bbox.bottom)
	bbox.bottom = points[i].py;
      if (points[i].py > bbox.top)
	bbox.top = points[i].py;
      if (points[i].px > bbox.right)
	bbox.right = points[i].px;
      if (points[i].px < bbox.left)
	bbox.left = points[i].px;
    }
    // make the bounding box 3% bigger than the shape
    float xdiff = bbox.left - bbox.right;
    float ydiff = bbox.top - bbox.bottom;
    xdiff *= 0.03;
    ydiff *= 0.03;
    bbox.top += ydiff;
    bbox.bottom -= ydiff;
    bbox.left += xdiff;
    bbox.right -= xdiff;
    numPoints = x;
};		



bool jPoly::pointInPoly(P2D click) {
  P2D one, two;
  float slope, testY;
  int i;
  int timesCrossed=0;
  int adder=0;
  int numberOfPoints;
		
    // first test vs bbox if it is not in that no need to go further
  if (click.px < bbox.left || click.px > bbox.right || click.py > bbox.top || click.py < bbox.bottom)
    return false;
	
  for (i=0; i<numPoints-1; i++)  {
	one.px = points[i].px;
	one.py = points[i].py;
	two.px = points[i+1].px;
	two.py = points[i+1].py;
	/*
	 * you only qualify to be tested if click.px is between one.px and two.px
	 */
	if ((one.px>=click.px && two.px<click.px) ||
	    (one.px<=click.px && two.px>click.px)) {
	  // don't need to protect against division by zero -- 
	  // one.px and two.px must be unequal to get here
	  slope = (two.py-one.py)/(two.px-one.px);
	  testY = (slope*(click.px - one.px) + one.py);
	  if (JPDEBUG)
	    printf("slope,testY= %f,%f  click.py= %f",slope,testY,click.py);
	  if (testY > click.py && testY < bbox.top)
	    timesCrossed++;
	}
	else if (JPDEBUG)
	  printf("Not Tested 1.px,tst.px,2.px %f,%f,%f",one.px,click.px,two.px);
      }
			
    if (JPDEBUG)
      printf("ptInPoly crosses = %i",timesCrossed);
			
    /* need to account for the case where the test line runs through a vertex
       this will produce 2 crosses (one from each ray of the vertex)
       test only the vertical axis if the x's are the same and the test y is below
       the poly point y -- then the test line runs through that point 
    */
    for (int i = 0; i < numPoints; i++)
      if (approxEqual(click.px,points[i].px) && click.py < points[i].py)
	adder++;

    if (adder > 0)
      timesCrossed -= (adder / 2);
		
    // if timesCrossed is odd then the point is in the polygon
    if (timesCrossed % 2 == 1)
      return true;		
    else
      return false;
};
		
bool jPoly::approxEqual(float x, float y)  {
  double z = x - y;
  if (z < 0)
    z *= -1.0;
  z *= 1000.0;
  if (z > 1.0)
    return false;
  else
    return true;
};
