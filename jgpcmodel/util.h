#ifndef util_h 
#define util_h

#include <math.h>

// 2D point on the map
typedef struct {
  double px;
  double py;
} dP2D;

typedef struct {
  float px;
  float py;
} P2D;


typedef struct {
  float left;
  float bottom;
  float right;
  float top;
} jRect;


typedef struct {
  int obsType;
  float px;
  float py;
  float pwidth;
  float plength;
  float pheight;
  int identifier;
  float reliability;
} BOGEY;



#endif


