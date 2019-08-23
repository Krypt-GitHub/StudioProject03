	#include "PistolGO.h"
#include "../Source/Application.h"
#include "GOList.h"
#include "../Ray.h"

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
}

void PistolGO::Update(double dt)
{
	m_v3storeDir = gl.FetchGO(GameObject::GO_PLAYER)->m_v3dir;
	m_v3storePos = gl.FetchGO(GameObject::GO_PLAYER)->transform.position;

	static bool m_bLBDown = false;
	static bool m_bRBDown = false;

	if (m_bisPickUp)
	{
		if (Application::GetMouseDown(0) && !m_bLBDown)
		{
			GameObject *go = gl.FetchGO();
			go->type = GameObject::GO_BULLET;
			go->SetActive(true);
			go->m_bGravity = false;
			
			//Ray *rae = new Ray(m_v3storeDir, m_v3storePos);
			//Vector3 targetPos;

			//for (auto go2 : gl.m_goList)
			//{
			//	if (go2->type == GameObject::GO_PISTOL || go == go2 || go2->type == GameObject::GO_NONE || go2->type == GameObject::GO_PLAYER || go2->type == GameObject::GO_BULLET || !go2->GetActive())
			//		continue;

			//	if (rae->IntersectionOBB(go2->obb, targetPos))
			//	{
			//		go->transform.position = Vector3(transform.position.x, transform.position.y + 1.5, transform.position.z);
			//		go->transform.scale.Set(0.3, 0.3, 0.3);
			//		go->m_v3dir = (targetPos - go->transform.position).Normalized();
			//		go->m_v3vel = (targetPos - go->transform.position).Normalized() * 200.f;
			//		break;
			//	}
			//}

			go->transform.position = Vector3(transform.position.x, transform.position.y + 1.5, transform.position.z);
			go->transform.scale.Set(0.3, 0.3, 0.3);
			go->m_v3dir = m_v3storeDir;
			go->m_v3vel = m_v3storeDir * 200.f;

			go->obb.SetScale(Vector3(0.15, 0.15, 0.15));
			go->obb.UpdateAxis(Vector3(1, 0, 0), Vector3(0, 0, 1));
			go->obb.pos = transform.position;
			go->obb.RotateAxis(0, Vector3(0, 1, 0));

			m_bLBDown = true;
		}
		else if (!Application::GetMouseDown(0) && m_bLBDown)
		{
			m_bLBDown = false;
		}

		if (Application::GetMouseDown(1) && !m_bRBDown)
		{
			m_bisPickUp = false;

			// Changing it back to a dynamic object
			SetStatic(false);

			m_bLBDown = true;
		}
		else if (!Application::GetMouseDown(1) && m_bRBDown)
		{
			m_bLBDown = false;
		}
	}

	//std::cout << m_v3vel << std::endl;
}
