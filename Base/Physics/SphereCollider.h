#ifndef SPHERE_COLLIDER_H
#define SPHERE_COLLIDER_H

#include "Collider.h"

class SphereCollider : public Collider
{
private:
	Vector3 center;
	float radius;
public:
	SphereCollider();
	~SphereCollider();
};

#endif
