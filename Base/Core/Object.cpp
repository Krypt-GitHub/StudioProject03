#include "Object.h"

Object::Object()
	: hidden(false)
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
