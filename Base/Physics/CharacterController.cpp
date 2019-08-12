#include "CharacterController.h"

CharacterController::CharacterController()
{
}

CharacterController::~CharacterController()
{
}

void CharacterController::Move()
{
}

void CharacterController::SimpleMove(Vector3 speed)
{
	transform.position += speed;
}
