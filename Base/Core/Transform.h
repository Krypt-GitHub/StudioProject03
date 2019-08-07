#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Component.h"

class Transform : public Component
{
public:
	Vector3 position;
	float rotation;
	Vector3 scale;

	Transform();
	~Transform();
};

#endif
