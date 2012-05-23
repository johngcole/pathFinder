#ifndef __DRAWSTUFFGRAPHICS_H_
#define __DRAWSTUFFGRAPHICS_H_

#include <boost/thread/mutex.hpp>
#include "Graphics.h"

class DrawstuffGraphics : public Graphics {
public:
	DrawstuffGraphics(Config *config, StatusVariables *status);
	~DrawstuffGraphics();

	void Start();
	void ChangeCameraView(Position3D &pos, Attitude &att);

private:
	static void DStuffThread(void *arg);

	boost::mutex _cameraMutex;
	Position3D _reqCameraPos;
	Attitude _reqCameraView;
};


#endif /* DRAWSTUFFGRAPHICS_H_ */
