#include "DrawstuffHelpers.h"

// skip over whitespace and comments in a stream.

static void skipWhiteSpace(const char *filename, FILE *f) {
	int c, d;
	for (;;) {
		c = fgetc(f);
		if (c == EOF)
			printf("unexpected end of file in \"%s\"", filename);

		// skip comments
		if (c == '#') {
			do {
				d = fgetc(f);
				if (d == EOF)
					printf("unexpected end of file in \"%s\"", filename);
			} while (d != '\n');
			continue;
		}

		if (c > ' ') {
			ungetc(c, f);
			return;
		}
	}
}

// read a number from a stream, this return 0 if there is none (that's okay
// because 0 is a bad value for all PPM numbers anyway).

static int readNumber(const char *filename, FILE *f) {
	int c, n = 0;
	for (;;) {
		c = fgetc(f);
		if (c == EOF)
			printf("unexpected end of file in \"%s\"", filename);
		if (c >= '0' && c <= '9')
			n = n * 10 + (c - '0');
		else {
			ungetc(c, f);
			return n;
		}
	}
}

Image::Image(const char *filename) {
	FILE *f = fopen(filename, "rb");
	if (!f)
		printf("Can't open image file `%s'", filename);

	// read in header
	if (fgetc(f) != 'P' || fgetc(f) != '6')
		printf("image file \"%s\" is not a binary PPM (no P6 header)", filename);
	skipWhiteSpace(filename, f);

	// read in image parameters
	image_width = readNumber(filename, f);
	skipWhiteSpace(filename, f);
	image_height = readNumber(filename, f);
	skipWhiteSpace(filename, f);
	int max_value = readNumber(filename, f);

	// check values
	if (image_width < 1 || image_height < 1)
		printf("bad image file \"%s\"", filename);
	if (max_value != 255)
		printf("image file \"%s\" must have color range of 255", filename);

	// read either nothing, LF (10), or CR,LF (13,10)
	int c = fgetc(f);
	if (c == 10) {
		// LF
	} else if (c == 13) {
		// CR
		c = fgetc(f);
		if (c != 10)
			ungetc(c, f);
	} else
		ungetc(c, f);

	// read in rest of data
	image_data = new byte[image_width * image_height * 3];
	if (fread(image_data, image_width * image_height * 3, 1, f) != 1)
		printf("Can not read data from image file `%s'", filename);
	fclose(f);
}

Image::~Image() {
	delete[] image_data;
}

//***************************************************************************
// Texture object.

Texture::Texture(const char *filename) {
	image = new Image(filename);
	glGenTextures(1, &name);
	glBindTexture(GL_TEXTURE_2D, name);

	// set pixel unpacking mode
	glPixelStorei(GL_UNPACK_SWAP_BYTES, 0);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);

	// glTexImage2D (GL_TEXTURE_2D, 0, 3, image->width(), image->height(), 0,
	//		   GL_RGB, GL_UNSIGNED_BYTE, image->data());
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image->width(), image->height(),
			GL_RGB, GL_UNSIGNED_BYTE, image->data());

	// set texture parameters - will these also be bound to the texture???
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}

Texture::~Texture() {
	delete image;
	glDeleteTextures(1, &name);
}

void Texture::bind(int modulate) {
	glBindTexture(GL_TEXTURE_2D, name);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, modulate ? GL_MODULATE
			: GL_DECAL);
}

/////////////////////////////////
//////   C Interface Helpers
//////////////////////////////////

// light vector. LIGHTZ is implicitly 1
#define LIGHTX (1.0f)
#define LIGHTY (0.4f)

// ground and sky
#define SHADOW_INTENSITY (0.65f)
#define GROUND_R (0.5f) 	// ground color for when there's no texture
#define GROUND_G (0.5f)
#define GROUND_B (0.3f)

// sets lighting and texture modes, sets current color
static void setupDrawingMode(Texture *text) {
	glEnable ( GL_LIGHTING);
	if (text != NULL) {
		glEnable ( GL_TEXTURE_2D);
		text->bind(1);
		glEnable ( GL_TEXTURE_GEN_S);
		glEnable ( GL_TEXTURE_GEN_T);
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		static GLfloat s_params[4] = { 1.0f, 1.0f, 0.0f, 1 };
		static GLfloat t_params[4] = { 0.817f, -0.817f, 0.817f, 1 };
		glTexGenfv(GL_S, GL_OBJECT_PLANE, s_params);
		glTexGenfv(GL_T, GL_OBJECT_PLANE, t_params);
	} else {
		glDisable ( GL_TEXTURE_2D);
	}
}

void setTransform(const float pos[3], const float R[12]) {
	GLfloat matrix[16];
	matrix[0] = R[0];
	matrix[1] = R[4];
	matrix[2] = R[8];
	matrix[3] = 0;
	matrix[4] = R[1];
	matrix[5] = R[5];
	matrix[6] = R[9];
	matrix[7] = 0;
	matrix[8] = R[2];
	matrix[9] = R[6];
	matrix[10] = R[10];
	matrix[11] = 0;
	matrix[12] = pos[0];
	matrix[13] = pos[1];
	matrix[14] = pos[2];
	matrix[15] = 1;
	glPushMatrix();
	glMultMatrixf(matrix);
}

void drawBox(const float sides[3]) {
	float lx = sides[0] * 0.5f;
	float ly = sides[1] * 0.5f;
	float lz = sides[2] * 0.5f;

	// sides
	glBegin ( GL_TRIANGLE_STRIP);
	glNormal3f(-1, 0, 0);
	glVertex3f(-lx, -ly, -lz);
	glVertex3f(-lx, -ly, lz);
	glVertex3f(-lx, ly, -lz);
	glVertex3f(-lx, ly, lz);
	glNormal3f(0, 1, 0);
	glVertex3f(lx, ly, -lz);
	glVertex3f(lx, ly, lz);
	glNormal3f(1, 0, 0);
	glVertex3f(lx, -ly, -lz);
	glVertex3f(lx, -ly, lz);
	glNormal3f(0, -1, 0);
	glVertex3f(-lx, -ly, -lz);
	glVertex3f(-lx, -ly, lz);
	glEnd();

	// top face
	glBegin ( GL_TRIANGLE_FAN);
	glNormal3f(0, 0, 1);
	glVertex3f(-lx, -ly, lz);
	glVertex3f(lx, -ly, lz);
	glVertex3f(lx, ly, lz);
	glVertex3f(-lx, ly, lz);
	glEnd();

	// bottom face
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0, 0, -1);
	glVertex3f(-lx, -ly, -lz);
	glVertex3f(-lx, ly, -lz);
	glVertex3f(lx, ly, -lz);
	glVertex3f(lx, -ly, -lz);
	glEnd();
}

void setShadowDrawingMode() {
	glDisable ( GL_LIGHTING);
	glDisable ( GL_TEXTURE_2D);
	glColor3f(GROUND_R * SHADOW_INTENSITY, GROUND_G * SHADOW_INTENSITY,
			GROUND_B * SHADOW_INTENSITY);
	glDepthRange(0, 0.9999);
}

void setShadowTransform() {
	GLfloat matrix[16];
	for (int i = 0; i < 16; i++)
		matrix[i] = 0;
	matrix[0] = 1;
	matrix[5] = 1;
	matrix[8] = -LIGHTX;
	matrix[9] = -LIGHTY;
	matrix[15] = 1;
	glPushMatrix();
	glMultMatrixf(matrix);
}

void drawCylinder(float l, float r, float zoffset) {
	int i;
	float tmp, ny, nz, a, ca, sa;
	const int n = 24; // number of sides to the cylinder (divisible by 4)

	l *= 0.5;
	a = float(M_PI * 2.0) / float(n);
	sa = (float) sin(a);
	ca = (float) cos(a);

	// draw cylinder body
	ny = 1;
	nz = 0; // normal vector = (0,ny,nz)
	glBegin ( GL_TRIANGLE_STRIP);
	for (i = 0; i <= n; i++) {
		glNormal3d(ny, nz, 0);
		glVertex3d(ny * r, nz * r, l + zoffset);
		glNormal3d(ny, nz, 0);
		glVertex3d(ny * r, nz * r, -l + zoffset);
		// rotate ny,nz
		tmp = ca * ny - sa * nz;
		nz = sa * ny + ca * nz;
		ny = tmp;
	}
	glEnd();

	// draw top cap
	glShadeModel ( GL_FLAT);
	ny = 1;
	nz = 0; // normal vector = (0,ny,nz)
	glBegin ( GL_TRIANGLE_FAN);
	glNormal3d(0, 0, 1);
	glVertex3d(0, 0, l + zoffset);
	for (i = 0; i <= n; i++) {
		//if (i == 1 || i == n / 2 + 1)
		//	setColor(color[0] * 0.75f, color[1] * 0.75f, color[2] * 0.75f,
		//			color[3]);
		glNormal3d(0, 0, 1);
		glVertex3d(ny * r, nz * r, l + zoffset);
		//if (i == 1 || i == n / 2 + 1)
		//	setColor(color[0], color[1], color[2], color[3]);

		// rotate ny,nz
		tmp = ca * ny - sa * nz;
		nz = sa * ny + ca * nz;
		ny = tmp;
	}
	glEnd();

	// draw bottom cap
	ny = 1;
	nz = 0; // normal vector = (0,ny,nz)
	glBegin(GL_TRIANGLE_FAN);
	glNormal3d(0, 0, -1);
	glVertex3d(0, 0, -l + zoffset);
	for (i = 0; i <= n; i++) {
		//if (i == 1 || i == n / 2 + 1)
		//	setColor(color[0] * 0.75f, color[1] * 0.75f, color[2] * 0.75f,
		//			color[3]);
		glNormal3d(0, 0, -1);
		glVertex3d(ny * r, nz * r, -l + zoffset);
		//if (i == 1 || i == n / 2 + 1)
		//	setColor(color[0], color[1], color[2], color[3]);

		// rotate ny,nz
		tmp = ca * ny + sa * nz;
		nz = -sa * ny + ca * nz;
		ny = tmp;
	}
	glEnd();
}

/////////////////////////////////
//////////////////////////////////


extern "C" void dshSetColor(float r, float g, float b, float alpha) {
	GLfloat light_ambient[4], light_diffuse[4], light_specular[4];
	light_ambient[0] = r * 0.3f;
	light_ambient[1] = g * 0.3f;
	light_ambient[2] = b * 0.3f;
	light_ambient[3] = alpha;
	light_diffuse[0] = r * 0.7f;
	light_diffuse[1] = g * 0.7f;
	light_diffuse[2] = b * 0.7f;
	light_diffuse[3] = alpha;
	light_specular[0] = r * 0.2f;
	light_specular[1] = g * 0.2f;
	light_specular[2] = b * 0.2f;
	light_specular[3] = alpha;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, light_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, light_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, light_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 5.0f);

	if (alpha < 1) {
		glEnable ( GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	} else {
		glDisable ( GL_BLEND);
	}
}

extern "C" void dshDrawBox(Texture *text, const float pos[3],
		const float R[12], const float sides[3]) {
	setupDrawingMode(text);
	glShadeModel ( GL_FLAT);
	setTransform(pos, R);
	drawBox(sides);
	glPopMatrix();

	setShadowDrawingMode();
	setShadowTransform();
	setTransform(pos, R);
	drawBox(sides);
	glPopMatrix();
	glPopMatrix();
	glDepthRange(0, 1);
}

extern "C" void dshDrawBoxD(Texture *text, const double pos[3],
		const double R[12], const double sides[3]) {
	int i;
	float pos2[3], R2[12], fsides[3];
	for (i = 0; i < 3; i++)
		pos2[i] = (float) pos[i];
	for (i = 0; i < 12; i++)
		R2[i] = (float) R[i];
	for (i = 0; i < 3; i++)
		fsides[i] = (float) sides[i];
	dshDrawBox(text, pos2, R2, fsides);
}

extern "C" void dshDrawCylinder(Texture *text, const float pos[3],
		const float R[12], float length, float radius) {
	setupDrawingMode(text);
	glShadeModel ( GL_SMOOTH);
	setTransform(pos, R);
	drawCylinder(length, radius, 0);
	glPopMatrix();

	setShadowDrawingMode();
	setShadowTransform();
	setTransform(pos, R);
	drawCylinder(length, radius, 0);
	glPopMatrix();
	glPopMatrix();
	glDepthRange(0, 1);
}

extern "C" void dshDrawCylinderD(Texture *text, const double pos[3],
		const double R[12], double length, double radius) {
	int i;
	float pos2[3], R2[12];
	for (i = 0; i < 3; i++)
		pos2[i] = (float) pos[i];
	for (i = 0; i < 12; i++)
		R2[i] = (float) R[i];
	dshDrawCylinder(text, pos2, R2, length, radius);
}

extern "C" void dshDrawLine(Texture *text, const float pos1[3],
		const float pos2[3]) {
	setupDrawingMode(text);
	glDisable ( GL_LIGHTING);
	glLineWidth(2);
	glShadeModel ( GL_FLAT);
	glBegin ( GL_LINES);
	glVertex3f(pos1[0], pos1[1], pos1[2]);
	glVertex3f(pos2[0], pos2[1], pos2[2]);
	glEnd();
}

extern "C" void dshDrawLineD(Texture *text, const double pos1[3],
		const double pos2[3]) {
	int i;
	float fpos1[3], fpos2[3];
	for (i = 0; i < 3; i++) {
		fpos1[i] = (float) pos1[i];
		fpos2[i] = (float) pos2[i];
	}
	dshDrawLine(text, fpos1, fpos2);
}

