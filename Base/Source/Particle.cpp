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

void Particle::SetActive(bool _active)
{
	active = _active;
}

bool Particle::GetActive()
{
	return active;
}

void Particle::SetLifeSpan(float _life)
{
	life = _life;
}

float Particle::GetLifeSpan()
{
	return life;
}
