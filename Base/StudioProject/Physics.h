#ifndef PHYSICS_H
#define PHYSICS_H

#include "GOList.h"

class Physics
{
public:
	GOList &gl = GOList::GetInstance();

	Physics();
	~Physics();

	void UpdateGO(double dt);
	void CollisionResponse(GameObject *go1, GameObject *go2);
	Vector3 apply_forces(GameObject* object) const;
};

#endif