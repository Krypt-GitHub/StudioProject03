#ifndef OBJECT_H
#define OBJECT_H

#include "Vector3.h"
#include <string>
class Object 
{
private:
public:
	Object();
	~Object();

	std::string name;
	bool hidden;

	static void DestroyInstantly(Object* obj);

	//std::string ReturnName();
};

#endif
