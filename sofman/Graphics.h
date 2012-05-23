#ifndef __GRAPHICS_H_
#define __GRAPHICS_H_

#include <boost/thread/mutex.hpp>
#include <boost/foreach.hpp>

#include <iostream>
#include <colelib/Config.h>
#include <colelib/Stoppable.h>
#include <colelib/Attitude.h>
#include <colelib/Position3D.h>

#include "StatusVariables.h"
#include "KeyListener.h"

using namespace cole::thread;
using namespace cole::util;
using namespace cole::math::position;
using namespace std;

class Graphics : public Stoppable {
public:
	Graphics(Config *config, StatusVariables *status);
	~Graphics();

	// re-implements Stoppable functions
	virtual void Start() = 0;

	virtual void ChangeCameraView(Position3D &pos, Attitude &att) = 0;

	void AttachKeyListener(KeyListener *key_listen);
	void RemoveKeyListener(KeyListener *key_listen);

protected:
	void postKeyDown(int key);
	void postKeyUp(int key);
	void postKeyPress(int key);
	Config *_config;
	StatusVariables *_status;

private:
	boost::mutex _mutex;
	vector<KeyListener*> _keyListeners;
};


#endif /* __GRAPHICS_H_ */
