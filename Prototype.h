/*
 * Prototype.h
 *
 *  Created on: 2015年9月7日
 *      Author: root
 */

#ifndef PROTOTYPE_H_
#define PROTOTYPE_H_

#include "Component.h"
#include "Iterator.h"
#include "Proto.h"

class Prototype : public Component {
public:
	Prototype() {}
	virtual ~Prototype()
	{
		std::map<std::string, Component*>::iterator itr = _mapComponent.begin();
			for (; itr != _mapComponent.end(); itr++)
				delete itr->second;
	}

	// a composite node implementation
	virtual void add(Component* component)
	{
		_mapComponent[component->getName()] = component;
	}

	// clone itself
	virtual Component* clone(void)
	{
		// create the prototype
		Prototype* prototype = new Prototype;
		prototype->setComment(getComment());
		prototype->setName(getName());

		// deep clone the map
		for (auto c : _mapComponent) {
			Component* component = c.second->clone();
			prototype->add(component);
		}

		return prototype;
	}

	// extent the component list
	Component* extent(Component* component)
	{
		// not need extent
		if (!component->getValid())
			return component;

		// clone a new one
		Component* clone = component->clone();
		_mapComponent[clone->getName()] = clone;
		clone->setNext(component);
		return clone;
	}

	virtual int length() const
	{
		int sum = 0;
		for (auto c : _mapComponent) {
			if (c.second->getRepeated())
				sum += sizeof(uint16_t);

			sum += c.second->length();
		}

		if (getNext()) sum += getNext()->length();
		return sum;
	}

	virtual bool parseFromArray(const char* data, int size)
	{
		std::map<std::string, Component*>::iterator itr = _mapComponent.begin();
		for (; itr != _mapComponent.end(); itr++) {
			int16_t times = 0;
			if (itr->second->getRepeated()) {
				if (size < static_cast<int>(sizeof(int16_t)))
					return false;

				memcpy(&times, data, sizeof(int16_t));
				data += sizeof(int16_t);
				size -= sizeof(int16_t);
			}

			for (int16_t i = 1; i < times; i++) {
				Component* component = itr->second->clone();
				component->setNext(itr->second);
				itr->second = component;
			}

			if (!itr->second->parseFromArray(data, size))
				return false;

			data += itr->second->length();
			size -= itr->second->length();
		}

		if (getNext() && !getNext()->parseFromArray(data, size))
			return false;

		return true;
	}

	virtual bool serializeToArray(char* data, int size) const
	{
		for (auto c : _mapComponent) {
			if (c.second->getRepeated()) {
				if (size < static_cast<int>(sizeof(int16_t)))
					return false;

				int16_t times = c.second->times();
				memcpy(data, &times, sizeof(int16_t));
				data += sizeof(int16_t);
				size -= sizeof(int16_t);
			}

			if (!c.second->serializeToArray(data, size))
				return false;

			data += c.second->length();
			size -= c.second->length();
		}

		if (getNext() && !getNext()->serializeToArray(data, size))
			return false;

		return true;
	}

	template <typename T>
	T getValue(const std::string& name);

	template <typename T>
	T addValue(const std::string& name, T value);

	// override operator [] to make interface short
	Component* operator[](const std::string& name)	{ return getComponent(name); }

private:
	Component* getComponent(const std::string& name)
	{
		std::map<std::string, Component*>::iterator itr = _mapComponent.find(name);
		if (itr != _mapComponent.end()) return itr->second;
		std::cout << "unknown field name" << std::endl;
		return NULL;
	}

private:
	std::map<std::string, Component*> _mapComponent;	// the map of component
};

template <typename T>
inline T Prototype::getValue(const std::string& name)
{
	// unknown field name
	Component* component = getComponent(name);
	if (!component) return T();

	// accessed through the iterator
	ProtoIterator<T> itr(component);
	return *itr;
}

template <>
inline Prototype* Prototype::getValue<Prototype*>(const std::string& name)
{
	// dynamic_cast<anything*>(NULL) == NULL
	return dynamic_cast<Prototype*>(getComponent(name));
}

template <typename T>
inline T Prototype::addValue(const std::string& name, T value)
{
	Component* component = NULL;
	if ((component = getComponent(name))) {
		component = extent(component);
		Proto<T>* proto = dynamic_cast<Proto<T>*>(component);
		if (proto) proto->setValue(value);
	}

	return T();	// useless
}

template <>
inline Prototype* Prototype::addValue<Prototype*>(const std::string& name, Prototype* value)
{
	Component* component = NULL;
	if ((component = getComponent(name))) {
		component = extent(component);
		component->setValid(true);
		return dynamic_cast<Prototype*>(component);
	}

	return NULL;
}

class PrototypeIterator : public Iterator {
public:
	PrototypeIterator(Component* Component) : Iterator(Component) {}
	virtual ~PrototypeIterator() {}
	Prototype* operator*(void)
	{
		return dynamic_cast<Prototype*>(cur());
	}
};

#endif /* PROTOTYPE_H_ */
