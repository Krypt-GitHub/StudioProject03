#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Object.h"
#include "../Core/Transform.h"

class GameObject : public Object
{
private:
	bool isActive;
public:

	enum GO_TYPE
	{
		GO_NONE = 0,
		GO_PISTOL,
		GO_TOTAL,
	};

	// Members
	GO_TYPE type;
	bool isStatic;
	float mass;

	Transform transform;


	// Functions
	GameObject(GO_TYPE typeValue = GO_NONE);
	~GameObject();


	void SetActive(bool _active);
};

#endif

