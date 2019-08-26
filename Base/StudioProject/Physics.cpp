#include "Physics.h"
#include "../Source/Application.h"
#include "EnemyGO.h"
#include "SceneManager.h"

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

				//Collision
				for (std::vector<GameObject *>::iterator it2 = it + 1; it2 != gl.m_goList.end(); ++it2)
				{
					GameObject *go2 = (GameObject *)*it2;
					if (go2->GetActive())
					{
						if (go1->obb.GetCollision(go2->obb))
						{
							CollisionResponse(go1, go2);
							if (!go1->GetActive())
								break;
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
	if (go1->obb.isEnabled && go2->obb.isEnabled)
	{
		switch (go1->type)
		{
		case GameObject::GO_PLAYER:
			switch (go2->type)
			{
			case GameObject::GO_WALL:
				//Stop them from going through walls
				break;
			case GameObject::GO_FLOOR:
				go1->m_bGravity = false;
				go1->transform.position.y = 19;
				break;
			}
			break;
		case GameObject::GO_PISTOL:
			switch (go2->type)
			{
			case GameObject::GO_WALL:
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
			case GameObject::GO_CEILING:
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
				}
				else
				{
					static_cast<EnemyGO*>(go2)->GunOnHand = static_cast<PistolGO*>(go1);
					static_cast<PistolGO*>(go1)->attachedGO = go2;
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
		case GameObject::GO_BULLET:
			switch (go2->type)
			{
			case GameObject::GO_PLAYER:
				go1->SetActive(false);
				//Game over code
				//Pseudo - fall over, restart scene
				SceneManager::SetSceneID(0);
				break;
			case GameObject::GO_ENEMY:
				go1->SetActive(false);
				go2->SetActive(false);
				--EnemyCount;
				break;
			case GameObject::GO_WALL:
				go1->SetActive(false);
				break;
			case GameObject::GO_FLOOR:
				go1->SetActive(false);
				break;
			}
			go1->m_v3vel.y += 0.981;
			break;
		}
		break;
	case GameObject::GO_PBULLET:
		switch (go2->type)
		{
		case GameObject::GO_PLAYER:
			go1->SetActive(false);
			break;
		case GameObject::GO_ENEMY:
			//go1->SetActive(false);
			//go2->SetActive(false);
			break;
		case GameObject::GO_WALL:
			go1->SetActive(false);
			break;
		case GameObject::GO_FLOOR:
			go1->SetActive(false);
			break;
		}
		break;
	case GameObject::GO_EBULLET:
		switch (go2->type)
		{
		case GameObject::GO_PLAYER:
			go1->SetActive(false);
			break;
		case GameObject::GO_ENEMY:
			switch (go2->type)
			{
			case GameObject::GO_FLOOR:
				go1->m_bGravity = false;
				break;
			}
			break;
		default:
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
