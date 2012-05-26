#include "DrawstuffHelpers.h"


// skip over whitespace and comments in a stream.

static void skipWhiteSpace (const char *filename, FILE *f)
{
  int c,d;
  for(;;) {
    c = fgetc(f);
    if (c==EOF) printf ("unexpected end of file in \"%s\"",filename);

    // skip comments
    if (c == '#') {
      do {
	d = fgetc(f);
	if (d==EOF) printf ("unexpected end of file in \"%s\"",filename);
      } while (d != '\n');
      continue;
    }

    if (c > ' ') {
      ungetc (c,f);
      return;
    }
  }
}


// read a number from a stream, this return 0 if there is none (that's okay
// because 0 is a bad value for all PPM numbers anyway).

static int readNumber (const char *filename, FILE *f)
{
  int c,n=0;
  for(;;) {
    c = fgetc(f);
    if (c==EOF) printf ("unexpected end of file in \"%s\"",filename);
    if (c >= '0' && c <= '9') n = n*10 + (c - '0');
    else {
      ungetc (c,f);
      return n;
    }
  }
}


Image::Image (const char *filename)
{
  FILE *f = fopen (filename,"rb");
  if (!f) printf ("Can't open image file `%s'",filename);

  // read in header
  if (fgetc(f) != 'P' || fgetc(f) != '6')
    printf ("image file \"%s\" is not a binary PPM (no P6 header)",filename);
  skipWhiteSpace (filename,f);

  // read in image parameters
  image_width = readNumber (filename,f);
  skipWhiteSpace (filename,f);
  image_height = readNumber (filename,f);
  skipWhiteSpace (filename,f);
  int max_value = readNumber (filename,f);

  // check values
  if (image_width < 1 || image_height < 1)
    printf ("bad image file \"%s\"",filename);
  if (max_value != 255)
    printf ("image file \"%s\" must have color range of 255",filename);

  // read either nothing, LF (10), or CR,LF (13,10)
  int c = fgetc(f);
  if (c == 10) {
    // LF
  }
  else if (c == 13) {
    // CR
    c = fgetc(f);
    if (c != 10) ungetc (c,f);
  }
  else ungetc (c,f);

  // read in rest of data
  image_data = new byte [image_width*image_height*3];
  if (fread (image_data,image_width*image_height*3,1,f) != 1)
    printf ("Can not read data from image file `%s'",filename);
  fclose (f);
}


Image::~Image()
{
  delete[] image_data;
}

//***************************************************************************
// Texture object.

Texture::Texture (const char *filename)
{
  image = new Image (filename);
  glGenTextures (1,&name);
  glBindTexture (GL_TEXTURE_2D,name);

  // set pixel unpacking mode
  glPixelStorei (GL_UNPACK_SWAP_BYTES, 0);
  glPixelStorei (GL_UNPACK_ROW_LENGTH, 0);
  glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei (GL_UNPACK_SKIP_ROWS, 0);
  glPixelStorei (GL_UNPACK_SKIP_PIXELS, 0);

  // glTexImage2D (GL_TEXTURE_2D, 0, 3, image->width(), image->height(), 0,
  //		   GL_RGB, GL_UNSIGNED_BYTE, image->data());
  gluBuild2DMipmaps (GL_TEXTURE_2D, 3, image->width(), image->height(),
		     GL_RGB, GL_UNSIGNED_BYTE, image->data());

  // set texture parameters - will these also be bound to the texture???
  glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		   GL_LINEAR_MIPMAP_LINEAR);

  glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}


Texture::~Texture()
{
  delete image;
  glDeleteTextures (1,&name);
}


void Texture::bind (int modulate)
{
  glBindTexture (GL_TEXTURE_2D,name);
  glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
	     modulate ? GL_MODULATE : GL_DECAL);
}

