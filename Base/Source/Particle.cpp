#include "Particle.h"

Particle::Particle(PARTICLE_TYPE typeValue)
	: type(typeValue),
	active(false),
	scale(1, 1, 1)
{
}


Particle::~Particle()
{
}
