#ifndef __GRAPHICS_H_
#define __GRAPHICS_H_

#include <boost/thread/mutex.hpp>
#undef Bool
#include <boost/foreach.hpp>

#include <iostream>
#include <colelib/Config.h>
#include <colelib/Stoppable.h>
#include <colelib/Attitude.h>
#include <colelib/Position3D.h>

#include "StatusVariables.h"
#include "KeyListener.h"
#include "ODECar.h"

using namespace cole::thread;
using namespace cole::util;
using namespace cole::math::position;
using namespace std;

class Graphics : public Stoppable {
public:
	Graphics(Config *config, StatusVariables *status);
	virtual ~Graphics();

	// re-implements Stoppable functions
	virtual void Start() = 0;

	virtual void ChangeCameraView(Position3D &pos, Attitude &att) = 0;

	void AttachODECar(ODECar *car);
	void AttachKeyListener(KeyListener *key_listen);
	void RemoveKeyListener(KeyListener *key_listen);

protected:
	void postKeyDown(short key);
	void postKeyUp(short key);
	Config *_config;
	StatusVariables *_status;
	ODECar *_car;

private:
	boost::mutex _mutex;
	vector<KeyListener*> _keyListeners;
};


#endif /* __GRAPHICS_H_ */
