#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Component.h"

class Transform : public Component
{
public:
	Vector3 position;
	Vector3 scale;
	float rotation;

	Transform();
	void SetTransform(Vector3 _position, Vector3 _scale, float _rotation);
	~Transform();
};

#endif
