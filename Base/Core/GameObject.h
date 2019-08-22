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
		GLASS_01,
		GLASS_02,
		GLASS_03,
		GLASS_04,
		GLASS_05,
		GLASS_06,
		GLASS_07,
		GLASS_08,
		GLASS_09,
		GLASS_10,
		GLASS_11,
		GLASS_12,
		GLASS_13,
		GLASS_14,
		GLASS_15,
		GLASS_16,
		GLASS_17,
		GLASS_18,
		GLASS_19,
		GLASS_20,
		GO_PLAY,
		GO_LOGO,
		GO_PLAYER,
		GO_ENEMY,
		GO_PISTOL,
		GO_BULLET,
		GO_FLOOR,
		GO_WALL,
		GO_CEILING,
		GO_GLASS,
		GO_TOTAL,
	};

	// Members
	GO_TYPE type;

	Vector3 m_v3dir;
	Vector3 m_v3vel;

	Transform transform;
	Collider obb;

	bool m_bGravity;

	// Functions
	GameObject(GO_TYPE typeValue = GO_NONE);
	virtual ~GameObject();

	void SetActive(bool _active);
	bool GetActive();
	void SetStatic(bool _static);
	bool GetStatic();
	void SetMass(float _mass);
	float GetMass();
};

#endif

