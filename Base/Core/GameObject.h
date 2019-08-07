#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Object.h"
#include "../Core/Component.h"
#include "../Core/Transform.h"

class GameObject : public Object
{

public:

	enum GO_TYPE
	{
		GO_NONE = 0,
		GO_PLAYER,
		GO_BALL,
		GO_PISTOL,
		GO_BULLET,
		GO_TOTAL,
	};

	// Members
	GO_TYPE type;
	bool isActive;
	bool isStatic;
	float mass;

	Transform transform;


	// Functions
	GameObject(GO_TYPE typeValue = GO_NONE);
	~GameObject();


	void SetActive(bool _active);
	//Component GetComponent(type);
	//void GetComponent<Component>();
	void AddComponent(Component _type);
};

#endif

