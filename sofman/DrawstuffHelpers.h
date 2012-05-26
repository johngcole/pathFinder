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

#include <math.h>

typedef unsigned char byte;

class Image {
	int image_width, image_height;
	byte *image_data;
public:
	Image(const char *filename);
	// load from PPM file
	~Image();
	int width() {
		return image_width;
	}
	int height() {
		return image_height;
	}
	byte *data() {
		return image_data;
	}
};

class Texture {
	Image *image;
	GLuint name;
public:
	Texture(const char *filename);
	~Texture();
	void bind(int modulate);
};

extern "C" {

void dshSetColor(float r, float g, float b, float alpha);
void dshDrawBox(Texture *text, const float pos[3], const float R[12], const float sides[3]);
void dshDrawBoxD(Texture *text, const double pos[3], const double R[12], const double sides[3]);
void dshDrawCylinder(Texture *text, const float pos[3], const float R[12], float length,
		float radius);
void dshDrawCylinderD(Texture *text, const double pos[3], const double R[12], double length,
		double radius);
void dshDrawLine(Texture *text, const float pos1[3], const float pos2[3]);
void dshDrawLineD(Texture *text, const double pos1[3], const double pos2[3]);

}

#endif /* DRAWSTUFF_H_ */
