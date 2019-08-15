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
				//Resetting their velocity if they are static
				if (go1->GetStatic())
				{
					go1->m_v3vel.SetZero();
					go1->m_v3acc.SetZero();
				}

				if (!go1->GetStatic())
				{
					//Gravity
					if (go1->m_bisOnGround)
						go1->m_v3vel.y = 0;

					//Updating position
					Vector3 new_pos = go1->transform.position + go1->m_v3vel * dt + go1->m_v3acc * (dt * dt * 0.5);
					Vector3 new_acc = apply_forces(go1);
					Vector3 new_vel = go1->m_v3vel + (go1->m_v3acc + new_acc) * (dt * 0.5);
					go1->transform.position = new_pos;
					go1->m_v3vel = new_vel;
					go1->m_v3acc = new_acc;
					
					//Collision
					for (auto go2 : gl.m_goList)
					{
						if (go1->obb.getCollision(go2->obb))
						{
							CollisionResponse(go1, go2);
						}
					}
				}

				//Updating OBB position
				go1->obb.upDatePos(go1->transform.position);
			}	
		}
	}
}

void Physics::CollisionResponse(GameObject *go1, GameObject *go2)
{
	//GO1 will always be dynamic objects
	switch (go1->type)
	{
	case GameObject::GO_PLAYER:
		switch (go2->type)
		{
		case GameObject::GO_FLOOR:
			go1->m_bisOnGround = true;
			break;
		}
		break;
	case GameObject::GO_ENEMY:
		switch (go2->type)
		{
		case GameObject::GO_FLOOR:
			go1->m_bisOnGround = true;
			break;
		}
		break;
	case GameObject::GO_PISTOL:
		switch (go2->type)
		{
		case GameObject::GO_FLOOR:
			//go1->m_bisOnGround = true;
			go1->m_v3vel.y *= -1;
			break;
		}
		break;
	case GameObject::GO_BULLET:
		switch (go2->type)
		{
		case GameObject::GO_FLOOR:
			go1->SetActive(false);
			break;
		}
		break;
	default:
		break;
	}
}

Vector3 Physics::apply_forces(GameObject* object) const
{
	//Gravity
	Vector3 grav_acc(0, -9810, 0);

	//if (object->m_bisOnGround)
	//{
	//	grav_acc = Vector3(0, 0, 0);
	//}
	//else
	//{
	//	grav_acc = Vector3(0, -9810, 0);
	//}
	
	return grav_acc;
} 