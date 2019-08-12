#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "../Core/Component.h"

class Rigidbody : public Component
{
public:
	// Members
	bool enableGravity;
	Vector3 velocity;


	Rigidbody();
	~Rigidbody();
};

#endif
