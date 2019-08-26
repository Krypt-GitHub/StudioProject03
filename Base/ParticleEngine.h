#pragma once
#include <vector>
#include"../Common/Source/MatrixStack.h"
#include "../Base/StudioProject/SceneBase.h"
#define MAXPARTICLES  10000
class ParticleEngine : public SceneBase
{
	int m_particleCount = 0;
	Particle* particle = nullptr;
public:
	static ParticleEngine* PEngine;
	static ParticleEngine* GetInstance()
	{
		if (PEngine == nullptr)
			PEngine = new ParticleEngine;
		return PEngine;

	}

	ParticleEngine()
	{
		SceneBase::Init();
	};
	~ParticleEngine() {};
	
	std::vector<Particle*>m_pList;
	Particle* getParticle(void)
	{
		for (std::vector<Particle*>::iterator it = m_pList.begin(); it != m_pList.end(); it++)
		{
			Particle* part = (Particle*)* it;
			if (!part->active)
			{
				part->active = true;
				m_particleCount++;
				return part;
			}
		}
		for (unsigned i = 0; i < 10; i++)
		{
			Particle* part = new Particle;
			m_pList.push_back(part);
		}
		Particle* part = m_pList.back();
		part->active = true;
		m_particleCount++;
		return part;
	}
	void SpawnParticle(GameObject * go,Particle::PARTICLE_TYPE type)
	{
		if (m_particleCount < MAXPARTICLES)
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
			particle = getParticle();
			particle->type = type;
			particle->life = 1.f;
			particle->vel = Vector3(Math::RandFloatMinMax(-5.f, 5.f), Math::RandFloatMinMax(-5.f, 5.f), Math::RandFloatMinMax(-5.f, 5.f));
			particle->pos = go->transform.position;
			particle->active = true;
			particle->scale = Vector3(0.4f, 0.4f, 0.4f);
			break;
		case Particle::PA_GUNSHATTER:
			particle = getParticle();
			particle->type = type;
			particle->life = 2.2f;
			particle->vel = Vector3(Math::RandFloatMinMax(-10.f, 10.f), Math::RandFloatMinMax(-5.f, 15.f), Math::RandFloatMinMax(-20.f, 0));
			particle->pos = go->transform.position;
			particle->active = true;
			particle->scale= Vector3(1.2f, 1.2f, 1.2f);
			break;
		case Particle::PA_BULLET:
			break;
		case Particle::PA_WALKING:
			break;
		case Particle::PA_ENEMYSHATTER:
			particle = getParticle();
			particle->type = type;
			particle->life = 1.2f;
			particle->vel = Vector3(Math::RandFloatMinMax(-10.f, 10.f), Math::RandFloatMinMax(0.f, 25.f), Math::RandFloatMinMax(-20.f, 0));
			particle->pos = go->transform.position;
			particle->active = true;
			particle->scale = Vector3(1.2f, 1.2f, 1.2f);
			particle->gotGravity = true;
			break;
		case Particle::PA_TOTAL:
			break;
		default:
			break;
		}

		}



	}
	
	void updateParticle(double dt) {

		for (auto go : m_pList)
		{
			if (go->active)
			{
				if (go->gotGravity)
					go->vel += Vector3(0, -2.45f) * dt;
				go->pos += go->vel * dt;
				go->life -= dt;
				if (go->life <= 0)
				{
					go->active = false;
					m_particleCount--;
				}
			}
		}
	}
};

