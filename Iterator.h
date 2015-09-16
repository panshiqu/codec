/*
 * Iterator.h
 *
 *  Created on: 2015年9月10日
 *      Author: root
 */

#ifndef ITERATOR_H_
#define ITERATOR_H_

#include "Component.h"

class Iterator {
public:
	Iterator(Component* component) : _begin(component), _current(component) {}
	virtual ~Iterator() {}

	virtual void begin(void)	{ _current = _begin; }
	virtual void next(void)		{ _current = _current->getNext(); }
	virtual bool end(void)		{ return _current ? false : true; }
	virtual Component* cur(void)	{ return _current; }

private:
	Component* _begin = NULL;		// record list begin node
	Component* _current = NULL;	// record list current node
};

#endif /* ITERATOR_H_ */
