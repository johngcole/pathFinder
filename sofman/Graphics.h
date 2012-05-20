#ifndef __GRAPHICS_H_
#define __GRAPHICS_H_

#include <boost/thread/mutex.hpp>
#include <boost/foreach.hpp>

#include <iostream>
#include <colelib/Stoppable.h>
#include <colelib/Attitude.h>
#include <colelib/Position3D.h>

#include "Drawer.h"
#include "KeyListener.h"

using namespace cole::thread;
using namespace cole::math::position;
using namespace std;

class Graphics : public Stoppable {
public:
	Graphics();
	~Graphics();

	// re-implements Stoppable functions
	virtual void Start() = 0;

	virtual void ChangeCameraView(Position3D &pos, Attitude &att) = 0;

	void AttachDrawer(Drawer *drawer);
	void RemoveDrawer(Drawer *drawer);
	void AttachKeyListener(KeyListener *key_listen);
	void RemoveKeyListener(KeyListener *key_listen);

protected:
	void postKeyDown(int key);
	void postKeyUp(int key);
	void postKeyPress(int key);
	void postDraw();

private:
	boost::mutex _mutex;
	vector<KeyListener*> _keyListeners;
	vector<Drawer*> _drawers;
};


#endif /* __GRAPHICS_H_ */
