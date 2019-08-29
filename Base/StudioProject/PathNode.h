#pragma once

#include "Vector3.h"
#include "../Physics/Collider.h"
#include "../Core/Transform.h"

class PathNode
{
public:
	int m_inodeStatus;
	int id;
	Transform transform;
	Collider obb;

	PathNode();
	~PathNode();

	void SetTransform(Vector3 _pos, Vector3 _scale);
	void SetOBB(Vector3 _obbposition, Vector3 _oobscale);
};

