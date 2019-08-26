#pragma once
#include "../Base/Source/Particle.h"
#include "Core/GameObject.h"
#include <vector>
#include"../Common/Source/MatrixStack.h"
#include "../Base/StudioProject/SceneBase.h"
#define MAXPARTICLES = 10000
class ParticleEngine : public SceneBase
{
	static ParticleEngine* PEngine;
	Particle* particle =NULL;
public:
	std::vector<Particle*>m_pList;
	static ParticleEngine* GetInstance()
	{
		if (PEngine == nullptr)
			PEngine = new ParticleEngine;

		return PEngine;

	}

	ParticleEngine();
	~ParticleEngine();

	void SpawnParticle(GameObject * go,Particle::PARTICLE_TYPE type = Particle::PA_NONE )
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
			particle = new Particle(type);
			particle->life = 0.2f;
			particle->vel = Vector3(Math::RandFloatMinMax(-10.f, 10.f), Math::RandFloatMinMax(-10.f, 10.f), Math::RandFloatMinMax(-10.f, 10.f));
			particle->pos = go->transform.position;
			particle->active = true;
			particle->scale = Vector3(0.4f, 0.4f, 0.4f);
			particle->gotGravity=false;
			break;
		case Particle::PA_GUNSHATTER:
			particle = new Particle(type);
			particle->life = 2.2f;
			particle->vel = Vector3(Math::RandFloatMinMax(-10.f, 10.f), Math::RandFloatMinMax(-5.f, 15.f), Math::RandFloatMinMax(-20.f, 0));
			particle->pos = go->transform.position;
			particle->active = true;
			particle->scale= Vector3(1.2f, 1.2f, 1.2f);
			particle->gotGravity=true;
			break;
		case Particle::PA_BULLET:
			break;
		case Particle::PA_WALKING:
			break;
		case Particle::PA_ENEMYSHATTER:
			particle = new Particle(type);
			particle->life = 1.2f;
			particle->vel = Vector3(Math::RandFloatMinMax(-10.f, 10.f), Math::RandFloatMinMax(0.f, 25.f), Math::RandFloatMinMax(-20.f, 0));
			particle->pos = go->transform.position;
			particle->active = true;
			particle->scale= Vector3(1.2f, 1.2f, 1.2f);
			particle->gotGravity=true;
			break;
		case Particle::PA_TOTAL:
			break;
		default:
			break;
		}
		if(particle !=nullptr)
			m_pList.push_back(particle);
		
		
		
		
	}
	void renderParticle(MS& ms) {
		for (auto go : m_pList)
		{
			switch (go->type)
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
			
				ms.PushMatrix();
				ms.Translate(go->pos.x,go->pos.y,go->pos.z);
				ms.Scale(go->scale.x,go->scale.y,go->scale.z);
				RenderMesh(meshList[GEO_CUBE],false,false,false);	
				ms.PopMatrix();
				break;
			case Particle::PA_GUNSHATTER:
				ms.PushMatrix();
				ms.Translate(go->pos.x, go->pos.y, go->pos.z);
				ms.Scale(go->scale.x, go->scale.y, go->scale.z);
				RenderMesh(meshList[GEO_CUBE], false, false, false);
				ms.PopMatrix();
				break;
			case Particle::PA_BULLET:
				break;
			case Particle::PA_WALKING:
				break;
			case Particle::PA_ENEMYSHATTER:
				ms.PushMatrix();
				ms.Translate(go->pos.x, go->pos.y, go->pos.z);
				ms.Scale(go->scale.x, go->scale.y, go->scale.z);
				RenderMesh(meshList[GEO_CUBE], false, false, false);
				ms.PopMatrix();
				break;
			default:
				break;
			}
		}
	}
	void updateParticle(double dt)	{

		for (auto go : m_pList)
		{
			if (go->active)
			{
				if (go->gotGravity)
					go->vel += Vector3(0, -2.45) * dt;
				go->pos += go->vel * dt;
				go->life -= dt;
				if (go->life <= 0)
				{
					go->active = false;

				}
			}
		}
	}
};

