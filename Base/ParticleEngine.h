#pragma once
#include "../Base/Source/Particle.h"
class ParticleEngine
{
	static ParticleEngine* PEngine;
	Particle* particle;
public:
	static ParticleEngine* GetInstance()
	{
		if (PEngine == nullptr)
			PEngine = new ParticleEngine;
		return PEngine;

	}

	ParticleEngine();
	~ParticleEngine();

	void SpawnParticle(Particle::PARTICLE_TYPE type = Particle::PA_NONE)
	{
		switch (type)
		{
		case Particle::PA_NONE:
			break;
		case Particle::PA_RAINDROP:
			break;
		case Particle::PA_SPLATTER:
			break;
		case Particle::PA_SMOKE:
			break;
		case Particle::PA_SNOW:
			break;
		case Particle::PA_GUNSMOKE:
			
			break;
		case Particle::PA_GUNSHATTER:
			break;
		case Particle::PA_TOTAL:
			break;
		default:
			break;
		}
	}
};

