/*
 * Component.h
 *
 *  Created on: 2015年9月8日
 *      Author: root
 */

#ifndef COMPONENT_H_
#define COMPONENT_H_

#include <map>
#include <string>
#include <iostream>

class Component {
public:
	Component() {}
	virtual ~Component()
	{
		// recursive delete the next
		Component* component = getNext();
		if (component) delete component;
	}

	// a leaf(Proto) node call this woudn't got error
	virtual void add(Component* component) {}

	// clone itself
	virtual Component* clone(void) = 0;

	// getters
	const std::string& getName(void) const		{ return _name; }
	const std::string& getComment(void) const	{ return _comment; }
	Component* getNext(void) const				{ return _next; }
	bool getValid(void) const						{ return _valid; }

	// setters
	void setName(const std::string& name)			{ _name = name; }
	void setComment(const std::string& comment)	{ _comment = comment; }
	void setNext(Component* component)				{ _next = component; }
	void setValid(bool valid)							{ _valid = valid; }

private:
	std::string _name = "";		// component's name
	std::string _comment = "";	// component's description
	Component* _next = NULL;	// repeated, the next component
	bool _valid = false;			// artificial set the value
};

#endif /* COMPONENT_H_ */
