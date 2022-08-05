#include "PistolGO.h"
#include "../Source/Application.h"
#include "GOList.h"
#include "../Physics/Ray.h"
#include "../Core/ParticleEngine.h"
#include "EnemyGO.h"

PistolGO::PistolGO()
{
	std::cout << "Pistol GameObject Created" << std::endl;
}


PistolGO::~PistolGO()
{
	std::cout << "Pistol GameObject Destroyed" << std::endl;
}

GOList &gl = GOList::GetInstance();

void PistolGO::Init()
{
	m_ifireRate = 10;
	m_freloadSpeed = 15;
	m_imaxClip = 12;
	m_icurClip = m_imaxClip;
	m_bisPickUp = false;

	m_goStorePlayer = gl.FetchGO(GameObject::GO_PLAYER);
	m_frotation = 0.f;
	m_bCanFire = true;
	m_bRotateCrosshair = false;
	m_imaxClip = 4;
	m_icurClip = m_imaxClip;
	attachedGO = NULL;
}

void PistolGO::Update(double dt)
{
	Vector3 m_v3storeDir = gl.FetchGO(GameObject::GO_PLAYER)->m_v3dir;
	Vector3 m_v3storePos = gl.FetchGO(GameObject::GO_PLAYER)->transform.position;

	static bool m_bLBDown = false;
	static bool m_bRBDown = false;

	if (m_bisPickUp)
	{
		if (Application::GetMouseDown(0) && !m_bLBDown && m_bCanFire && m_icurClip > 0)
		{
			GameObject *go = gl.FetchGO();
			go->type = GameObject::GO_PBULLET;
			go->SetActive(true);
			go->m_bGravity = false;

			go->transform.position = Vector3(transform.position.x, transform.position.y + 1.5, transform.position.z) + Vector3(m_v3storeDir.x*2.25,0,m_v3storeDir.z*2.25f);
			go->transform.scale.Set(0.3, 0.3, 0.3);
			go->m_v3dir = m_v3storeDir;
			go->m_v3vel = m_v3storeDir * 200.f;
			for (int i = 0; i < 15; ++i)
			{
				ParticleEngine::GetInstance()->SpawnParticle(go, Particle::PA_GUNSMOKE);
			}
			go->obb.SetScale(Vector3(0.15, 0.15, 0.15));
			go->obb.UpdateAxis(Vector3(1, 0, 0), Vector3(0, 0, 1));
			go->obb.pos = transform.position;
			go->obb.RotateAxis(0, Vector3(0, 1, 0));
			//CSoundEngine::Getinstance()->PlayASound("Gunshot");
			m_frotation = 0;
			m_bRotateCrosshair = true;
			m_bCanFire = false;
			m_bLBDown = true;
			m_icurClip--;
		}
		else if (!Application::GetMouseDown(0) && m_bLBDown)
		{
			m_bLBDown = false;
		}

		if (Application::GetMouseDown(0) && !m_bLBDown && m_bCanFire && m_icurClip == 0)
		{
			m_bLBDown = true;
			//CSoundEngine::Getinstance()->PlayASound("pickup");
		}
		else if (!Application::GetMouseDown(0) && m_bLBDown)
		{
			m_bLBDown = false;
		}
	}

	if (m_bRotateCrosshair)
	{
		m_frotation -= 50 * dt;
		if (m_frotation <= -90)
		{
			m_bRotateCrosshair = false;
			m_bCanFire = true;
			//CSoundEngine::Getinstance()->PlayASound("pickup");
		}
	}

	if (attachedGO != NULL)
	{
		if (!attachedGO->GetActive())
		{
			this->SetStatic(false);
			attachedGO == NULL;
			static_cast<EnemyGO*>(attachedGO)->GunOnHand = NULL;
			m_bisPickUp = false;
		}
	}
}
