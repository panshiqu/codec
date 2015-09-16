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

	virtual int length() const = 0;
	virtual bool parseFromArray(const char* data, int size) = 0;
	virtual bool serializeToArray(char* data, int size) const = 0;

	// getters
	const std::string& getName(void) const		{ return _name; }
	const std::string& getComment(void) const	{ return _comment; }
	Component* getNext(void) const				{ return _next; }
	bool getValid(void) const						{ return _valid; }
	bool getRepeated(void) const					{ return _repeated; }

	// setters
	void setName(const std::string& name)			{ _name = name; }
	void setComment(const std::string& comment)	{ _comment = comment; }
	void setNext(Component* component)				{ _next = component; }
	void setValid(bool valid)							{ _valid = valid; }
	void setRepeated(bool repeated)					{ _repeated = repeated; }

	// get repeated times
	int16_t times(void) const	{ return (getNext() ? getNext()->times() : 0) + 1; }

private:
	std::string _name = "";		// component's name
	std::string _comment = "";	// component's description
	Component* _next = NULL;	// repeated, the next component
	bool _valid = false;			// artificial set the value
	bool _repeated = true;		// is it a repeated field
};

#endif /* COMPONENT_H_ */
