#include "DrawstuffGraphics.h"

Texture* DrawstuffGraphics::SkyText = NULL;
Texture* DrawstuffGraphics::GroundText = NULL;
Texture* DrawstuffGraphics::WoodText = NULL;
Texture* DrawstuffGraphics::CheckeredText = NULL;

DrawstuffGraphics::DrawstuffGraphics(Config *config, StatusVariables *status) :
	Graphics(config, status), _cameraMutex(), _reqCameraPos(Position3D::ZERO),
			_reqCameraView(Attitude::INVALID_ATT) {

}
DrawstuffGraphics::~DrawstuffGraphics() {
}

void DrawstuffGraphics::Start() {
	startThread(DrawstuffGraphics::DStuffThread, this);
}
void DrawstuffGraphics::ChangeCameraView(Position3D &pos, Attitude &att) {
	_cameraMutex.lock();
	_reqCameraPos = pos;
	_reqCameraView = att;
	_cameraMutex.unlock();
}

void DrawstuffGraphics::DStuffThread(void *arg) {
	DrawstuffGraphics *ds = (DrawstuffGraphics*) arg;
	Position3D cameraPos(Length::ZERO, Length::ZERO, Length::ONE_METER);
	Attitude cameraView(Angle::ZERO, Angle::ZERO, NorthBearingAngle::NORTH);

	// X11 display info
	Display *display = 0;
	int screen = 0;
	XVisualInfo *visual = 0; // best visual for openGL
	Colormap colormap = 0; // window's colormap
	Atom wm_protocols_atom = 0;
	Atom wm_delete_window_atom = 0;
	GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { LIGHTX, LIGHTY, 1.0, 0.0 };

	// window and openGL
	Window win = 0; // X11 window, 0 if not initialized
	GLXContext glx_context = 0; // openGL rendering context

	Logger::getInstance()->log("DStuff Graphics Thread starting...");

	// create X11 display connection
	display = XOpenDisplay(NULL);
	if (!display)
		Logger::getInstance()->log("can not open X11 display");
	screen = DefaultScreen(display);

	// get GL visual
	int attribListDblBuf[] = { GLX_RGBA, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, 16,
			GLX_RED_SIZE, 4, GLX_GREEN_SIZE, 4, GLX_BLUE_SIZE, 4, None };
	int attribList[] = { GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_RED_SIZE, 4,
			GLX_GREEN_SIZE, 4, GLX_BLUE_SIZE, 4, None };
	visual = glXChooseVisual(display, screen, attribListDblBuf);
	if (!visual)
		visual = glXChooseVisual(display, screen, attribList);
	if (!visual)
		Logger::getInstance()->log("no good X11 visual found for OpenGL");

	// create colormap
	colormap = XCreateColormap(display, RootWindow(display, screen),
			visual->visual, AllocNone);

	// initialize variables
	win = 0;
	glx_context = 0;

	// create the window
	XSetWindowAttributes attributes;
	attributes.background_pixel = BlackPixel(display, screen);
	attributes.colormap = colormap;
	attributes.event_mask = ButtonPressMask | ButtonReleaseMask | KeyPressMask
			| KeyReleaseMask | ButtonMotionMask | PointerMotionHintMask
			| StructureNotifyMask;
	win = XCreateWindow(display, RootWindow(display, screen), 50, 50,
			WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGHT, 0, visual->depth,
			InputOutput, visual->visual,
			CWBackPixel | CWColormap | CWEventMask, &attributes);

	// associate a GLX context with the window
	glx_context = glXCreateContext(display, visual, 0, GL_TRUE);
	if (!glx_context)
		Logger::getInstance()->log("can't make an OpenGL context");

	// set the window title
	XTextProperty window_name;
	window_name.value = (unsigned char *) "Sofman Sim";
	window_name.encoding = XA_STRING;
	window_name.format = 8;
	window_name.nitems = strlen((char *) window_name.value);
	XSetWMName(display, win, &window_name);

	// participate in the window manager 'delete yourself' protocol
	wm_protocols_atom = XInternAtom(display, "WM_PROTOCOLS", False);
	wm_delete_window_atom = XInternAtom(display, "WM_DELETE_WINDOW", False);
	if (XSetWMProtocols(display, win, &wm_delete_window_atom, 1) == 0)
		Logger::getInstance()->log("XSetWMProtocols() call failed");

	// pop up the window
	XMapWindow(display, win);
	XSync(display, win);
	glXMakeCurrent(display, win, glx_context);

	string tx_path("./");
	try {
		tx_path = ds->_config->getValue("PathToTextures");
	} catch (ConfigException ce) {
		Logger::getInstance()->log("DStuff: Can't get textures path");
	}
	if (tx_path[tx_path.length() - 1] != '/')
		tx_path += "/";
	string file = tx_path + "sky.ppm";
	SkyText = new Texture(file.c_str());
	file = tx_path + "ground.ppm";
	GroundText = new Texture(file.c_str());
	file = tx_path + "wood.ppm";
	WoodText = new Texture(file.c_str());
	file = tx_path + "checkered.ppm";
	CheckeredText = new Texture(file.c_str());

	try {
		while (true) {
			boost::this_thread::interruption_point();

			// read in and process all pending events for the main window
			XEvent event;
			while (XPending(display)) {
				XNextEvent(display, &event);
				if (event.type == KeyPress) {
					KeySym key;
					XLookupString(&event.xkey, NULL, 0, &key, 0);
					ostringstream oss;
					oss << "DStuff: KeyPress: ";
					oss << (short) key;
					Logger::getInstance()->log(oss.str().c_str());
					ds->postKeyDown((short) key);
				}
				if (event.type == KeyRelease) {
					KeySym key;
					XLookupString(&event.xkey, NULL, 0, &key, 0);
					ostringstream oss;
					oss << "DStuff: KeyRelease: ";
					oss << (short) key;
					Logger::getInstance()->log(oss.str().c_str());
					ds->postKeyUp((short) key);
				}
			}

			/////////////////////////////////////
			//dsDrawFrame(width, height, fn, pause && !singlestep);

			// setup stuff
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);
			glShadeModel(GL_FLAT);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);

			// setup viewport
			glViewport(0, 0, WINDOW_SIZE_WIDTH, WINDOW_SIZE_HEIGHT);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			const float vnear = 0.1f;
			const float vfar = 100.0f;
			const float k = 0.8f; // view scale, 1 = +/- 45 degrees
			if (WINDOW_SIZE_WIDTH >= WINDOW_SIZE_HEIGHT) {
				float k2 = float(WINDOW_SIZE_HEIGHT) / float(WINDOW_SIZE_WIDTH);
				glFrustum(-vnear * k, vnear * k, -vnear * k * k2, vnear * k
						* k2, vnear, vfar);
			} else {
				float k2 = float(WINDOW_SIZE_WIDTH) / float(WINDOW_SIZE_HEIGHT);
				glFrustum(-vnear * k * k2, vnear * k * k2, -vnear * k, vnear
						* k, vnear, vfar);
			}

			// setup lights. it makes a difference whether this is done in the
			// GL_PROJECTION matrix mode (lights are scene relative) or the
			// GL_MODELVIEW matrix mode (lights are camera relative, bad!).
			glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
			glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
			glColor3f(1.0, 1.0, 1.0);

			// clear the window
			glClearColor(0.5, 0.5, 0.5, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// snapshot camera position (in MS Windows it is changed by the GUI thread)
			ds->_cameraMutex.lock();
			if (ds->_reqCameraView != Attitude::INVALID_ATT) {
				cameraPos = ds->_reqCameraPos;
				cameraView = ds->_reqCameraView;
			}
			ds->_cameraMutex.unlock();

			// go to GL_MODELVIEW matrix mode and set the camera
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			setCamera(cameraPos.getX().getDoubleValue(Length::METERS),
					cameraPos.getY().getDoubleValue(Length::METERS),
					cameraPos.getZ().getDoubleValue(Length::METERS),
					cameraView.getYaw().getDoubleValue(Angle::DEGREES),
					cameraView.getPitch().getDoubleValue(Angle::DEGREES),
					cameraView.getRoll().getDoubleValue(Angle::DEGREES));

			// set the light position (for some reason we have to do this in model view.
			glLightfv(GL_LIGHT0, GL_POSITION, light_position);

			// draw the background (ground, sky etc)
			_drawSky_(cameraPos);
			_drawGround_();

			// leave openGL in a known state - flat shaded white, no textures
			glEnable(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			glShadeModel(GL_FLAT);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glColor3f(1, 1, 1);
			setColor(1, 1, 1, 1);

			// draw the rest of the objects. set drawing state first.
			//if (fn->step)
			//	fn->step(pause);

			////////////////////////////////////

			glFlush();
			glXSwapBuffers(display, win);
			XSync(display, 0);
		}
	} catch (boost::thread_interrupted const&) {
		// exiting grapics thread
		Logger::getInstance()->log("DStuff Graphics Thread exiting...");
	}

	delete SkyText;
	delete GroundText;
	delete WoodText;
	delete CheckeredText;
	glXDestroyContext(display, glx_context);
	XDestroyWindow(display, win);
	XSync(display, 0);
	XCloseDisplay(display);
	display = 0;
	win = 0;
	glx_context = 0;
}

void DrawstuffGraphics::_drawSky_(Position3D &camera) {
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	SkyText->bind(0);

	// make sure sky depth is as far back as possible
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(1, 1);

	const float ssize = 1000.0f;
	static float offset = 0.0f;
	const float sky_scale = 1.0f / 4.0f; // sky texture scale (1/size)
	const float sky_height = 1.0f; // sky height above viewpoint

	float camx = camera.getX().getDoubleValue(Length::METERS);
	float camy = camera.getY().getDoubleValue(Length::METERS);
	float camz = camera.getZ().getDoubleValue(Length::METERS);

	float x = ssize * sky_scale;
	float z = camz + sky_height;

	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	glTexCoord2f(-x + offset, -x + offset);
	glVertex3f(-ssize + camx, -ssize + camy, z);
	glTexCoord2f(-x + offset, x + offset);
	glVertex3f(-ssize + camx, ssize + camy, z);
	glTexCoord2f(x + offset, x + offset);
	glVertex3f(ssize + camx, ssize + camy, z);
	glTexCoord2f(x + offset, -x + offset);
	glVertex3f(ssize + camx, -ssize + camy, z);
	glEnd();

	offset = offset + 0.002f;
	if (offset > 1)
		offset -= 1;

	glDepthFunc(GL_LESS);
	glDepthRange(0, 1);
}

void DrawstuffGraphics::_drawGround_() {
	glDisable(GL_LIGHTING);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_TEXTURE_2D);
	GroundText->bind(0);

	const float gsize = 100.0f;
	const float offset = 0; // -0.001f; ... polygon offsetting doesn't work well
	const float ground_scale = 1.0f / 1.0f; // ground texture scale (1/size)
	const float ground_ofsx = 0.5; // offset of ground texture
	const float ground_ofsy = 0.5;

	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(-gsize * ground_scale + ground_ofsx, -gsize * ground_scale
			+ ground_ofsy);
	glVertex3f(-gsize, -gsize, offset);
	glTexCoord2f(gsize * ground_scale + ground_ofsx, -gsize * ground_scale
			+ ground_ofsy);
	glVertex3f(gsize, -gsize, offset);
	glTexCoord2f(gsize * ground_scale + ground_ofsx, gsize * ground_scale
			+ ground_ofsy);
	glVertex3f(gsize, gsize, offset);
	glTexCoord2f(-gsize * ground_scale + ground_ofsx, gsize * ground_scale
			+ ground_ofsy);
	glVertex3f(-gsize, gsize, offset);
	glEnd();

	glDisable(GL_FOG);
}



