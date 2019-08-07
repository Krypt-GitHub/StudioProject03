#ifndef BEHAVIOUR_H
#define BEHAVIOUS_H

#include "Component.h"

class Behaviour : public Component
{
public:
	bool enabled;

	Behaviour();
	~Behaviour();
};

#endif