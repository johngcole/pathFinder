/*
 * Drawstuff.h
 *
 *  Created on: May 25, 2012
 *      Author: gray
 */

#ifndef DRAWSTUFF_H_
#define DRAWSTUFF_H_

#include <stdio.h>
#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>

typedef unsigned char byte;

class Image {
  int image_width,image_height;
  byte *image_data;
public:
  Image (const char *filename);
  // load from PPM file
  ~Image();
  int width() { return image_width; }
  int height() { return image_height; }
  byte *data() { return image_data; }
};



class Texture {
  Image *image;
  GLuint name;
public:
  Texture (const char *filename);
  ~Texture();
  void bind (int modulate);
};


#endif /* DRAWSTUFF_H_ */
