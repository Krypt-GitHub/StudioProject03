#pragma once
#include "Light.h"

struct Particle
{
private:
public:
	enum PARTICLE_TYPE
	{
		PA_NONE = 0,
		PA_RAINDROP,
		PA_SPLATTER,
		PA_SMOKE,
		PA_SNOW,
		PA_GUNSMOKE,
		PA_GUNSHATTER,
		PA_BULLET,
		PA_WALKING,
		PA_ENEMYSHATTER,
		//PA_GLASSSHATTER,
	
		PA_TOTAL,
	};
	PARTICLE_TYPE type;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	bool active;
	float life;

	bool active;
	float life;
	bool gotGravity;

	Particle(PARTICLE_TYPE typeValue = PA_RAINDROP);
	~Particle();
};

