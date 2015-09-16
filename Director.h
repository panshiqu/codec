/*
 * Director.h
 *
 *  Created on: 2015年9月7日
 *      Author: root
 */

#ifndef DIRECTOR_H_
#define DIRECTOR_H_

#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "Prototype.h"
#include "Proto.h"
#include "Utils.h"

class Director {
private:
	Director() {}
	virtual ~Director()
	{
		std::map<std::string, Prototype*>::iterator itr = _mapPrototype.begin();
		for (; itr != _mapPrototype.end(); itr++)
			delete itr->second;
	}

	// noncopyable
	Director(const Director&);
	const Director& operator=(const Director&);

public:
	// singleton pattern
	static Director& getInstance(void)
	{
		static Director director;
		return director;
	}

	// load prototype from the protocol file
	bool loadPrototype(const char* filename)
	{
		std::ifstream file(filename);
		if (!file.is_open()) {
			std::cout << "open file error" << std::endl;
			return false;
		}

		std::string line;
		Prototype* prototype = NULL;
		while (getline(file, line)) {
			Utils::eraseCharacter(line, ' ');	// erase blank
			Utils::eraseCharacter(line, '\t');	// erase tabs
			if (line.empty()) continue;	// skip empty line
			if (line[0] == '#') continue;	// skip comment line

			// split the line
			std::vector<std::string> splits;
			Utils::splitString(splits, line, ",");

			if (splits[0] == "message") {
				prototype = new Prototype;
				prototype->setName(splits[1]);
				prototype->setComment(splits[2]);
				_mapPrototype.insert(make_pair(splits[1], prototype));
			} else {
				if (!prototype) {
					std::cout << "define message first" << std::endl;
					return false;
				}

				// create known type
				Component* component = NULL;
				if (splits[0] == "int32") {
					component = new Proto<int32_t>;
				} else if (splits[0] == "string") {
					component = new Proto<std::string>;
				} else if (splits[0] == "bool") {
					component = new Proto<bool>;
				} else if (splits[0] == "float") {
					component = new Proto<float>;
				} else if (splits[0] == "double") {
					component = new Proto<double>;
				} else if (splits[0] == "uint32") {
					component = new Proto<uint32_t>;
				} else if (splits[0] == "int64") {
					component = new Proto<int64_t>;
				} else if (splits[0] == "uint64") {
					component = new Proto<uint64_t>;
				}

				// custom type
				if (!component) {
					component = getPrototype(splits[0]);
					if (!component) {
						std::cout << "unknown custom type" << std::endl;
						return false;
					}

					// clone it for easy delete
					component = component->clone();
				}

				// add component
				component->setComment(splits[2]);
				component->setName(splits[1]);
				prototype->add(component);
			}
		}

		return true;
	}

	// get a copy of assigned prototype
	Prototype* clonePrototype(const std::string& name)
	{
		Prototype* prototype = NULL;
		if (!(prototype = getPrototype(name))) {
			std::cout << "unknown message name" << std::endl;
			return prototype;
		}

		// use Prototype rather than Component just meaning prototype pattern
		return dynamic_cast<Prototype*>(prototype->clone());
	}

private:
	Prototype* getPrototype(const std::string& name)
	{
		std::map<std::string, Prototype*>::iterator itr = _mapPrototype.find(name);
		if (itr != _mapPrototype.end()) return itr->second;
		std::cout << "unknown prototype name" << std::endl;
		return NULL;
	}

private:
	std::map<std::string, Prototype*> _mapPrototype;	// the map of prototype
};

#endif /* DIRECTOR_H_ */
