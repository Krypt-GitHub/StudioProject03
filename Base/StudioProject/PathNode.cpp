#include "PathNode.h"



PathNode::PathNode()
	: m_bobstructed(false)
{
}


PathNode::~PathNode()
{
}

void PathNode::SetTransform(Vector3 _pos, Vector3 _scale)
{
	transform.position = _pos;
	transform.scale = _scale;
	transform.rotation = 0;
}

void PathNode::SetOBB(Vector3 _obbposition, Vector3 _oobscale)
{
	obb.pos = _obbposition;
	obb.SetScale(_oobscale);
	obb.UpdateAxis(Vector3(1, 0, 0), Vector3(0, 0, 1));
}