#include "Physics.h"
#include "../Source/Application.h"
#include "EnemyGO.h"
#include "../Core/ParticleEngine.h"
#include "SceneManager.h"
#include "..//Core/SoundEngine.h"
#include "Level2Scene.h"

int Physics::EnemyCount = 0;

Physics::Physics()
{
}

Physics::~Physics()
{
}

void Physics::UpdateGO(double dt, PlayerGO* _player)
{
	float delta = dt;

	if (Application::GetShouldUpdate())
		delta *= 1.0f;
	else
		delta *= 0.01f;

	for (std::vector<GameObject *>::iterator it = gl.m_goList.begin(); it != gl.m_goList.end(); ++it)
	{
		GameObject *go1 = (GameObject *)*it;
		if (go1->GetActive())
		{
			if (go1->type == GameObject::GO_ENEMY)
			{
				static_cast<EnemyGO*>(go1)->Update(delta, _player);
			}
			if (go1->type == GameObject::GO_PISTOL)
			{
				static_cast<PistolGO*>(go1)->Update(delta);
			}

			//Resetting their velocity if they are static
			if (go1->GetStatic())
			{
				go1->m_v3vel.SetZero();
			}

			if (!go1->GetStatic())
			{
				//Gravity
				if (go1->m_bGravity)
					go1->m_v3vel.y += -0.981;
				//Updating position
				go1->transform.position += go1->m_v3vel * delta;

				//Constraints
				if (Math::FAbs(go1->transform.position.x) >= 1000 || Math::FAbs(go1->transform.position.y) >= 1000 || Math::FAbs(go1->transform.position.z) >= 1000)
					go1->SetActive(false);

				//Collision
				for (std::vector<GameObject *>::iterator it2 = it + 1; it2 != gl.m_goList.end(); ++it2)
				{
					GameObject *go2 = (GameObject *)*it2;
					if (go2->GetActive())
					{
						if (go1->obb.GetCollision(go2->obb))
						{
							if (!go1->GetActive())
								break;
							CollisionResponse(go1, go2);
						}
					}
				}
			}

			//Updating OBB position
			go1->obb.UpdatePos(go1->transform.position);
		}
	}
}

void Physics::CollisionResponse(GameObject *go1, GameObject *go2)
{
	//GO1 will always be dynamic objects
	if (go1->obb.isEnabled && go2->obb.isEnabled && go2->GetActive())
	{
		switch (go1->type)
		{
		case GameObject::GO_PLAYER:
			switch (go2->type)
			{
			case GameObject::GO_FLOOR:
				go1->m_bGravity = false;
				go1->transform.position.y = 19;
				break;
			case GameObject::GO_ENEMY:
				go2->SetActive(false);
				SceneManager::SetSceneID(4);
				break;
			}
			break;
		case GameObject::GO_PISTOL:
			switch (go2->type)
			{
			case GameObject::GO_WALL:
			case GameObject::GO_CEILING:
			case GameObject::GO_PILLAR:
			{
				Vector3 N = go2->obb.AxisX.Cross(go2->obb.AxisY);
				Vector3 w0minusb1 = go2->transform.position - go1->transform.position;
				if (w0minusb1.Dot(N) < 0)
					N = -N;
				if (go1->m_v3vel.Dot(N) < 0)
					break;

				Vector3 NP(N.y, -N.x);
				if (w0minusb1.Dot(N) < go1->transform.scale.x + go2->transform.scale.x * 0.5f &&
					Math::FAbs(w0minusb1.Dot(NP)) < go1->transform.scale.x + go2->transform.scale.y * 0.5f)
				{
					go1->m_v3vel -= (2 * go1->m_v3vel.Dot(go2->obb.AxisX.Cross(go2->obb.AxisY))) * go2->obb.AxisX.Cross(go2->obb.AxisY);
					go1->m_v3vel *= 0.3;
				}
			}
			break;
			case GameObject::GO_ENEMY:
				//Checking if pistol has enough velocity to consider collision
				if (go1->m_v3vel.Length() >= 10.f)
				{
					go1->SetActive(false);
					go2->SetActive(false);
					--EnemyCount;
					Level2Scene::m_ienemyNum--;
					for (int i = 0; i < 10; ++i)
					{
						ParticleEngine::GetInstance()->SpawnParticle(go1, Particle::PA_GUNSHATTER);
					}
					for (int i = 0; i < 80; ++i)
					{
						ParticleEngine::GetInstance()->SpawnParticle(go2, Particle::PA_ENEMYSHATTER);
					}
					CSoundEngine::Getinstance()->PlayASound("ESHATTER");
				}
				else
				{
					static_cast<EnemyGO*>(go2)->GunOnHand = static_cast<PistolGO*>(go1);
					static_cast<PistolGO*>(go1)->attachedGO = go2;
					static_cast<PistolGO*>(go1)->SetPickUp(true);
				}
				break;
			case GameObject::GO_FLOOR:
				if (go1->m_v3vel.y <= 0)
				{
					go1->m_v3vel.y *= -1;
					go1->m_v3vel *= 0.3;
				}
				go1->m_v3vel.y += 0.981;
				break;
			}
			break;
		case GameObject::GO_PBULLET:
			switch (go2->type)
			{
			case GameObject::GO_ENEMY:
				go1->SetActive(false);
				go2->SetActive(false);
				--EnemyCount;
				Level2Scene::m_ienemyNum--;
				for (int i = 0; i < 80; ++i)
				{
					ParticleEngine::GetInstance()->SpawnParticle(go2, Particle::PA_ENEMYSHATTER);
				}
				CSoundEngine::Getinstance()->PlayASound("ESHATTER");
				break;
			case GameObject::GO_WALL:
			case GameObject::GO_CEILING:
			case GameObject::GO_FLOOR:
			case GameObject::GO_PILLAR:
				go1->SetActive(false);
				break;
			}
			break;
		case GameObject::GO_EBULLET:
			switch (go2->type)
			{
			case GameObject::GO_PLAYER:
				go1->SetActive(false);
				SceneManager::SetSceneID(4);
				break;
			case GameObject::GO_WALL:
			case GameObject::GO_CEILING:
			case GameObject::GO_FLOOR:
			case GameObject::GO_PILLAR:
				go1->SetActive(false);
				break;
			}
			break;
		case GameObject::GO_ENEMY:
			switch (go2->type)
			{
			case GameObject::GO_FLOOR:
				go1->m_bGravity = false;
				go1->m_v3vel.y = 0;
				break;
			case GameObject::GO_ENEMY:
				static_cast<EnemyGO*>(go2)->m_bstartwalk = false;
				break;
			}
			break;
		}
	}
}

int Physics::GetEnemyCount()
{
	return EnemyCount;
}

void Physics::SetEnemyCount(int _count)
{
	EnemyCount = _count;
}
