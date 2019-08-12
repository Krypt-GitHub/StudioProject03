#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Object.h"
#include "../Core/Transform.h"
#include "../Physics/Collider.h"

class GameObject : public Object
{
private:
	bool m_bisActive;
public:

	enum GO_TYPE
	{
		GO_NONE = 0,
		GO_PISTOL,
		GO_BULLET,
		GO_TOTAL,
	};

	// Members
	GO_TYPE type;
	bool isStatic;
	float mass;

	Vector3 dir;

	Transform transform;
	Collider obb;

	// Functions
	GameObject(GO_TYPE typeValue = GO_NONE);
	~GameObject();

	void SetActive(bool _active);
	bool GetActive();
};

#endif

