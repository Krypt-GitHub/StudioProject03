#ifndef SPHERE_COLLIDER_H
#define SPHERE_COLLIDER_H

#include "Collider.h"

class SphereCollider : public Collider
{
private:
	float radius;
public:
	SphereCollider();
	~SphereCollider();
};

#endif
