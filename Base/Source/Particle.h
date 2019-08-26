#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector3.h"

class Particle
{
private:
	bool active;
	float life;
public:
	enum PARTICLE_TYPE
	{
		PA_NONE = 0,
		PA_RAINDROP,
		PA_SPLATTER,
		PA_SMOKE,
		PA_SNOW,
		PA_TOTAL,
	};
	PARTICLE_TYPE type;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;

	Particle(PARTICLE_TYPE typeValue = PA_RAINDROP);
	~Particle();

	void SetActive(bool _active);
	bool GetActive();
	void SetLifeSpan(float _life);
	float GetLifeSpan();
};

#endif
