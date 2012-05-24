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

/*
	Popular key assignments:
	Left arrow: -175
	Right arrow: -173
	Up arrow: -174
	Down arrow: -172
	Escape: -229
	Enter: -243
*/


	virtual void keyUp(short key) = 0;
	virtual void keyDown(short key) = 0;
};

#endif /* __KEYLISTENER_H_ */
