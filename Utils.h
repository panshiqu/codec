/*
 * Utils.h
 *
 *  Created on: 2015年9月8日
 *      Author: root
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <vector>
#include <string>
#include <iostream>

class Utils {
public:
	Utils();
	virtual ~Utils();

	// erase the assigned character
	static void eraseCharacter(std::string& source, const char character)
	{
		while (true) {
			size_t pos = source.find(character);
			if (pos == std::string::npos)
				break;

			source.erase(pos, 1);
		}
	}

	// split the string by the assigned delimiter
	static void splitString(std::vector<std::string>& splits, std::string source, std::string delimiter)
	{
		size_t index;
		while ((index = source.find(delimiter)) != std::string::npos) {
			splits.push_back(source.substr(0, index));
			source = source.substr(index+1, -1);
		}

		if (!source.empty())
			splits.push_back(source);
	}
};

#endif /* UTILS_H_ */
