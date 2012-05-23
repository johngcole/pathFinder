#include "DrawstuffGraphics.h"

DrawstuffGraphics::DrawstuffGraphics(Config *config, StatusVariables *status) :
Graphics(config, status),
_cameraMutex(),
_reqCameraPos(Position3D::ZERO),
_reqCameraView(Attitude::INVALID_ATT)
{

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
	DrawstuffGraphics *ds = (DrawstuffGraphics*)arg;

}

