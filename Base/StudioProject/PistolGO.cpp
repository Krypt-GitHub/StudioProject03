#include "PistolGO.h"
#include "../Source/Application.h"
#include "GOList.h"

PistolGO::PistolGO()
{
}


PistolGO::~PistolGO()
{

}

GOList &gl = GOList::GetInstance();

void PistolGO::Init()
{
	m_ifireRate = 10;
	m_freloadSpeed = 15;
	m_imaxClip = 12;
	m_icurClip = m_imaxClip;
	m_bisPickUp = false;

	//m_goStorePlayer = gl.FetchGO(GameObject::GO_PLAYER);
}

void PistolGO::Update(double dt)
{

	m_goStorePlayer = gl.FetchGO(GameObject::GO_PLAYER);
	m_v3storeDir = gl.FetchGO(GameObject::GO_PLAYER)->m_v3dir;

	

	static bool m_bLBDown = false;
	static bool m_bRBDown = false;

	if (m_bisPickUp)
	{
		if (Application::GetMouseDown(0) && !m_bLBDown)
		{
			GameObject *go = gl.FetchGO();
			go->SetActive(true);
			go->type = GameObject::GO_BULLET;
			go->m_v3dir = m_v3storeDir;
			go->transform.position = transform.position;
			go->transform.scale.Set(2, 2, 2);

			m_bLBDown = true;
		}
		else if (!Application::GetMouseDown(0) && m_bLBDown)
		{
			m_bLBDown = false;
		}

		//if (Application::GetMouseDown(1) && !m_bRBDown)
		//{
		//	//GameObject *player = gl.FetchGO(GameObject::GO_PLAYER);
		//	m_bisPickUp = false;

		//	//Throwing physics
		//	//weaponObject->m_v3vel = player->m_v3dir * 150000.f * dt;
		//	//float vel = 1000.f;
		//	//float horiVel = vel * cos(Math::DegreeToRadian(45.f));
		//	//float vertVel = vel * sin(Math::DegreeToRadian(45.f));
		//	//weaponObject->m_v3vel = Vector3(horiVel, vertVel);

		//	m_v3vel.SetZero();

		//	// Changing it back to a dynamic object
		//	SetStatic(false);

		//	m_bLBDown = true;
		//}
		//else if (!Application::GetMouseDown(1) && m_bRBDown)
		//{
		//	m_bLBDown = false;
		//}
	}

	if (Application::GetKeyDown('E') && (transform.position - m_goStorePlayer->transform.position).Length() < 1000)
	{
		m_bisPickUp = true;
	}

	//std::cout << transform.position << std::endl;
}