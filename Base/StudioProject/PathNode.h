#pragma once

#include "Vector3.h"
#include "../Physics/Collider.h"

class PathNode
{
private:
	Vector3 pos;
	Collider obb;
public:
	PathNode();
	~PathNode();
};

