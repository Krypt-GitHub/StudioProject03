#include "Object.h"

Object::Object()
{
}


Object::~Object()
{
}

void Object::DestroyInstantly(Object* _obj)
{
	delete _obj;
}

//std::string Object::ReturnName()
//{
//	return name;
//}
