#ifndef CHARACTER_CONTROLLER_H
#define CHARACTER_CONTROLLER_H

#include "Collider.h"
#include "../Core/Transform.h"

class CharacterController : public Collider
{
private:
	bool detectCollisions;
	bool isGrounded;
	float height;
	float minMoveDistance;
public:
	CharacterController();
	~CharacterController();

	Transform transform;

	void Move();
	void SimpleMove(Vector3 speed);
};

#endif
