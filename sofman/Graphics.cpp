#include "Graphics.h"

Graphics::Graphics() :
	Stoppable() {

}
Graphics::~Graphics() {
}

void Graphics::AttachDrawer(Drawer* drawer) {
	_mutex.lock();
	_drawers.push_back(drawer);
	_mutex.unlock();
}

void Graphics::RemoveDrawer(Drawer* drawer) {
	_mutex.lock();
	vector<Drawer*>::iterator it;
	it = find(_drawers.begin(), _drawers.end(), drawer);
	if (it != _drawers.end())
		_drawers.erase(it);
	_mutex.unlock();
}

void Graphics::AttachKeyListener(KeyListener* key_list) {
	_mutex.lock();
	_keyListeners.push_back(key_list);
	_mutex.unlock();
}

void Graphics::RemoveKeyListener(KeyListener* key_list) {
	_mutex.lock();
	vector<KeyListener*>::iterator it;
	it = find(_keyListeners.begin(), _keyListeners.end(), key_list);
	if (it != _keyListeners.end())
		_keyListeners.erase(it);
	_mutex.unlock();
}

void Graphics::postDraw() {
	_mutex.lock();
	Drawer* d;
	BOOST_FOREACH( d, _drawers) {
		d->draw();
	}
	_mutex.unlock();
}

void Graphics::postKeyDown(int key) {
	_mutex.lock();
	KeyListener* l;
	BOOST_FOREACH( l, _keyListeners) {
		l->keyDown(key);
	}
	_mutex.unlock();
}

void Graphics::postKeyUp(int key) {
	_mutex.lock();
	KeyListener* l;
	BOOST_FOREACH( l, _keyListeners) {
		l->keyUp(key);
	}
	_mutex.unlock();
}

void Graphics::postKeyPress(int key) {
	_mutex.lock();
	KeyListener* l;
	BOOST_FOREACH( l, _keyListeners) {
		l->keyPress(key);
	}
	_mutex.unlock();
}


