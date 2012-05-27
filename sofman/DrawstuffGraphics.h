#ifndef __DRAWSTUFFGRAPHICS_H_
#define __DRAWSTUFFGRAPHICS_H_

#include <boost/thread/mutex.hpp>

#include <string>


#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <GL/glx.h>

#include <colelib/Logger.h>
#include "Graphics.h"
#include "DrawstuffHelpers.h"

#define WINDOW_SIZE_WIDTH		420
#define WINDOW_SIZE_HEIGHT		380
#define LIGHTX (1.0f)
#define LIGHTY (0.4f)

using namespace std;
using namespace cole::util;

class DrawstuffGraphics : public Graphics {
public:
	DrawstuffGraphics(Config *config, StatusVariables *status);
	~DrawstuffGraphics();

	void Start();
	void ChangeCameraView(Position3D &pos, Attitude &att);

private:
	static void DStuffThread(void *arg);
	static void _drawGround_();
	static void _drawSky_(Position3D &camera);
	static void _setCamera_(Position3D &camera, Attitude &view);
	static void _drawCar_(ODECar *car);
	static Texture *SkyText, *GroundText, *WoodText, *CheckeredText;

	boost::mutex _cameraMutex;
	Position3D _reqCameraPos;
	Attitude _reqCameraView;
};


#endif /* DRAWSTUFFGRAPHICS_H_ */
