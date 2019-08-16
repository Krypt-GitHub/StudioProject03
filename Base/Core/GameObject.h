#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Object.h"
#include "../Core/Transform.h"
#include "../Physics/Collider.h"

class GameObject : public Object
{
private:
	bool m_bisActive;
	bool m_bisStatic;
	float m_fmass; 
public:

	enum GO_TYPE
	{
		GO_NONE = 0,
		GO_PLAYER,
		GO_ENEMY,
		GO_PISTOL,
		GO_BULLET,
		GO_FLOOR,
		GO_TOTAL,
	};

	// Members
	GO_TYPE type;

	Vector3 m_v3dir;
	Vector3 m_v3vel;
	Vector3 m_v3acc;

	Transform transform;
	Collider obb;

	bool m_bisOnGround;

	// Functions
	GameObject(GO_TYPE typeValue = GO_NONE);
	~GameObject();

	void SetActive(bool _active);
	bool GetActive();
	void SetStatic(bool _static);
	bool GetStatic();
	void SetMass(float _mass);
	float GetMass();
};

#endif

