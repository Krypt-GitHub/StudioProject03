#include "Transform.h"

Transform::Transform() 
	: position(1.f, 2.f, 1.f),
	rotation(0.f),
	scale(1.f, 1.f, 1.f)
{

}

void Transform::SetTransform(Vector3 _position, Vector3 _scale, float _rotation)
{
	position = _position;
	scale = _scale;
	rotation = _rotation;
}


Transform::~Transform()
{
}
