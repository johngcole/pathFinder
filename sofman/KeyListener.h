/*
 * KeyListener.h
 *
 *  Created on: May 20, 2012
 *      Author: gray
 */

#ifndef __KEYLISTENER_H_
#define __KEYLISTENER_H_

class KeyListener {
public:
	virtual void keyPress(int key) = 0;
	virtual void keyUp(int key) = 0;
	virtual void keyDown(int key) = 0;
};

#endif /* __KEYLISTENER_H_ */
