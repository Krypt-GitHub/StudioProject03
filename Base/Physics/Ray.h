#pragma once
#include "Vector3.h"
#include "../Physics/Collider.h"
class Ray
{
	Vector3 dir;
	Vector3 Pos;

public:
	Ray(Vector3 dir,Vector3 pos);
	~Ray();

	bool IntersectionOBB(Collider& box);
	bool IntersectionOBB(Collider & box, Vector3 & output);

};

