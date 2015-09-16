/*
 * Proto.h
 *
 *  Created on: 2015年9月8日
 *      Author: root
 */

#ifndef PROTO_H_
#define PROTO_H_

#include "Component.h"
#include "Iterator.h"

template <typename T>
class Proto : public Component {
public:
	Proto() {}
	virtual ~Proto() {}

	// clone itself
	virtual Component* clone(void)
	{
		Proto<T>* proto = new Proto<T>;
		proto->setComment(getComment());
		proto->setName(getName());
		return proto;
	}

	// getter and setter
	const T& getValue(void) const	{ return _value; }
	void setValue(const T& value)	{ _value = value; setValid(true); }

private:
	T _value = T();
};

template <typename T>
class ProtoIterator : public Iterator {
public:
	ProtoIterator(Component* component) : Iterator(component) {}
	virtual ~ProtoIterator() {}
	T operator*(void)
	{
		Proto<T>* proto = dynamic_cast<Proto<T>*>(cur());
		if (proto) return proto->getValue();
		std::cout << "type mismatch" << std::endl;
		return T();
	}
};

#endif /* PROTO_H_ */
