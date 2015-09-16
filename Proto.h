/*
 * Proto.h
 *
 *  Created on: 2015年9月8日
 *      Author: root
 */

#ifndef PROTO_H_
#define PROTO_H_

#include <string.h>

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

	virtual int length() const;
	virtual bool parseFromArray(const char* data, int size);
	virtual bool serializeToArray(char* data, int size) const;

	// getter and setter
	const T& getValue(void) const	{ return _value; }
	void setValue(const T& value)	{ _value = value; setValid(true); }

private:
	T _value = T();
};

template <typename T>
int Proto<T>::length() const
{
	return sizeof(T)*times();
}

template <>
int Proto<std::string>::length() const
{
	// worry about length > uint16_t
	int sum = _value.length()+sizeof(uint32_t);
	if (getNext()) sum += getNext()->length();
	return sum;
}

template <typename T>
bool Proto<T>::parseFromArray(const char* data, int size)
{
	if (size < sizeof(T))
		return false;

	memcpy(&_value, data, sizeof(T));
	if (getNext()) getNext()->parseFromArray(data+sizeof(T), size-sizeof(T));
	return true;
}

template <>
bool Proto<std::string>::parseFromArray(const char* data, int size)
{
	if (size < static_cast<int>(sizeof(uint32_t)))
		return false;

	uint32_t len = 0;
	memcpy(&len, data, sizeof(uint32_t));

	if (size < static_cast<int>(len+sizeof(uint32_t)))
		return false;

	_value.assign(data+sizeof(uint32_t), len);
	if (getNext()) getNext()->parseFromArray(data+len+sizeof(uint32_t), size-len-sizeof(uint32_t));
	return true;
}

template <typename T>
bool Proto<T>::serializeToArray(char* data, int size) const
{
	if (size < sizeof(T))
		return false;

	memcpy(data, &_value, sizeof(T));
	if (getNext()) getNext()->serializeToArray(data+sizeof(T), size-sizeof(T));
	return true;
}

template <>
bool Proto<std::string>::serializeToArray(char* data, int size) const
{
	uint32_t len = _value.length();
	if (size < static_cast<int>(len+sizeof(uint32_t)))
		return false;

	memcpy(data, &len, sizeof(uint32_t));
	memcpy(data+sizeof(uint32_t), _value.c_str(), len);
	if (getNext()) getNext()->serializeToArray(data+len+sizeof(uint32_t), size-len-sizeof(uint32_t));
	return true;
}

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
