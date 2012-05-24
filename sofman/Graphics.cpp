#include "Graphics.h"

Graphics::Graphics(Config *config, StatusVariables *status) :
	Stoppable() {
	_config = config;
	_status = status;
}
Graphics::~Graphics() {
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


void Graphics::postKeyDown(short key) {
	_mutex.lock();
	KeyListener* l;
	BOOST_FOREACH( l, _keyListeners) {
		l->keyDown(key);
	}
	_mutex.unlock();
}

void Graphics::postKeyUp(short key) {
	_mutex.lock();
	KeyListener* l;
	BOOST_FOREACH( l, _keyListeners) {
		l->keyUp(key);
	}
	_mutex.unlock();
}




