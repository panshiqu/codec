/*
 * main.cpp
 *
 *  Created on: 2015年9月7日
 *      Author: root
 */

#include "Director.h"

#include <memory>

using namespace std;

int main(void)
{
	Director::getInstance().loadPrototype("protocol.conf");

	Prototype* user = Director::getInstance().clonePrototype("User");
	if (!user) return true;

	// setter
	user->addValue<string>("username", "panshiqu");
	user->addValue("password", static_cast<string>("116227"));
	for (int i = 1; i < 10;  i++) {
		user->addValue<uint32_t>("score", i);
	}

	// getter
	cout << user->getValue<string>("username") << endl;
	cout << user->getValue<string>("password") << endl;
	cout << user->getValue<uint32_t>("index") << endl;	// default
	ProtoIterator<uint32_t> scoreItr((*user)["score"]);
	for (; !scoreItr.end(); scoreItr.next()) {
		cout << *scoreItr << endl;
	}

	delete user;	// don't forget this

	unique_ptr<Prototype> allUser(Director::getInstance().clonePrototype("AllUser"));
	if (!allUser) return true;

	// add message
	for (int i = 1; i < 3; i++) {
		// or allUser->addValue("users", static_cast<Prototype*>(NULL));
		Prototype* message = allUser->addValue<Prototype*>("users", NULL);
		if (!message) break;

		// setter
		message->addValue<string>("username", "panshiqu");
	}

	char* psz = new char[allUser->length()];
	cout << allUser->serializeToArray(psz, allUser->length()) << endl;
	unique_ptr<Prototype> allUserBak(Director::getInstance().clonePrototype("AllUser"));
	cout << allUserBak->parseFromArray(psz, allUser->length()) << endl;
	delete[] psz;

	PrototypeIterator userItr((*allUserBak)["users"]);
	for (; !userItr.end(); userItr.next()) {
		Prototype* message = *userItr;
		if (!message) break;

		// getter
		cout << message->getValue<string>("username") << endl;
	}
}

