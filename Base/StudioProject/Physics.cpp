#include "Physics.h"
#include "../Source/Application.h"

Physics::Physics()
{
}

Physics::~Physics()
{
}

void Physics::UpdateGO(double dt)
{
	for (auto go1 : gl.m_goList)
	{
		if (Application::GetShouldUpdate())
		{
			if (go1->GetActive())
			{
				go1->transform.position += go1->m_v3vel * dt;

				if (go1->type == GameObject::GO_FLOOR)
				{
					continue;
				}

				if (!go1->GetStatic())
				{
					//Gravity
					go1->m_v3vel.y += go1->GetMass() * -9.8 * dt;
				}

				for (auto go2 : gl.m_goList)
				{
					//Collision with floor
					if (go2->type == GameObject::GO_FLOOR)
					{
						if (go1->obb.getCollision(go2->obb))
						{
							CollisionResponse(go1, go2);
						}
					}
				}

				go1->obb.upDatePos(go1->transform.position);
			}
		}
	}
}

void Physics::CollisionResponse(GameObject *go1, GameObject *go2)
{
	switch (go1->type)
	{
	case GameObject::GO_PLAYER:
		switch (go2->type)
		{
		case GameObject::GO_BULLET:
			break;
		case GameObject::GO_FLOOR:
			go1->transform.position.y = go2->transform.position.y + go1->transform.scale.y;
			break;
		default:
			break;
		}
		break;
	case GameObject::GO_PISTOL:
		switch (go2->type)
		{
		case GameObject::GO_PISTOL:
			break;
		case GameObject::GO_BULLET:
			break;
		case GameObject::GO_FLOOR:
			go1->transform.position.y = go2->transform.position.y + go1->transform.scale.y;
			go1->m_v3vel *= 0.9;
			//go1->m_v3vel.y *= -1;
			//go1->transform.position.y = Math::Clamp(go1->transform.position.y, -10.f, 10000.f);
			break;
		default:
			break;
		}
		break;
	case GameObject::GO_BULLET:
		switch (go2->type)
		{
		case GameObject::GO_PLAYER:
			break;
		case GameObject::GO_PISTOL:
			break;
		case GameObject::GO_BULLET:
			break;
		//case GameObject::GO_FLOOR:
		//	go1->transform.position.y = go2->transform.position.y + go1->transform.scale.y;
		//	break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}