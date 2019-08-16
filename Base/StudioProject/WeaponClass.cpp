#include "WeaponClass.h"
#include "../Source/Application.h"

WeaponClass::WeaponClass()
	: m_ifireRate(10.f),
	m_freloadSpeed(5.f),
	m_imaxClip(10)
{
}


WeaponClass::~WeaponClass()
{
	if (weaponObject != NULL)
	{
		delete weaponObject;
		weaponObject = NULL;
	}
}

void WeaponClass::Init(const std::string _name, GameObject::GO_TYPE _type, int _fireRate, float _reloadSpeed, int _maxClip)
{
	weaponObject = new GameObject;

	weaponObject->name = _name;
	weaponObject->type = _type;
	weaponObject->transform.position.Set(0.f, 1000.f, 0.f);
	weaponObject->transform.scale.Set(1.f, 1.f, 1.f);
	weaponObject->SetMass(1);

	weaponObject->obb.upDateAxis(Vector3(1, 0, 0), Vector3(0, 0, 1));
	weaponObject->obb.upDatePos(weaponObject->transform.position);
	weaponObject->obb.setScale(Vector3(weaponObject->transform.scale.x * 1.5, weaponObject->transform.scale.y * 6.5, weaponObject->transform.scale.z * 10));

	m_ifireRate = _fireRate;
	m_freloadSpeed = _reloadSpeed;
	m_imaxClip = _maxClip;
	m_icurClip = _maxClip;
	m_bisPickUp = false;
	
	weaponObject->SetActive(true);

	gl.m_goList.push_back(weaponObject);
}

void WeaponClass::Update(double dt, Vector3 _dir)
{
	static bool m_bLBDown = false;
	static bool m_bRBDown = false;
	if (m_bisPickUp)
	{
		if (Application::GetMouseDown(0) && !m_bLBDown)
		{
			GameObject *go = gl.FetchGO();
			go->SetActive(true);
			go->type = GameObject::GO_BULLET;
			go->m_v3dir = _dir;
			go->transform.position = weaponObject->transform.position;
			go->transform.scale.Set(2, 2, 2);
			gl.m_goList.push_back(go);

			m_bLBDown = true;
		}
		else if (!Application::GetMouseDown(0) && m_bLBDown)
		{
			m_bLBDown = false;
		}

		if (Application::GetMouseDown(1) && !m_bRBDown)
		{
			GameObject *player = gl.FetchGO(GameObject::GO_PLAYER);
			m_bisPickUp = false;

			//Throwing physics
			//weaponObject->m_v3vel = player->m_v3dir * 150000.f * dt;
			//float vel = 1000.f;
			//float horiVel = vel * cos(Math::DegreeToRadian(45.f));
			//float vertVel = vel * sin(Math::DegreeToRadian(45.f));
			//weaponObject->m_v3vel = Vector3(horiVel, vertVel);

			weaponObject->m_v3vel.SetZero();
			std::cout << weaponObject->m_v3vel << std::endl;
			//Changing it back to a dynamic object
			weaponObject->SetStatic(false);

			m_bLBDown = true;
		}
		else if (!Application::GetMouseDown(1) && m_bRBDown)
		{
			m_bLBDown = false;
		}
	}
	std::cout << weaponObject->transform.position << std::endl;
}

void WeaponClass::SetPickUp(bool _isPickUp)
{
	m_bisPickUp = _isPickUp;
}

bool WeaponClass::GetPickUp()
{
	return m_bisPickUp;
}

void WeaponClass::UpdatePosition(Vector3 _playerPosition)
{
	m_v3storePosition = _playerPosition;
}

int WeaponClass::GetCurrClip()
{
	return m_icurClip;
}

GameObject* WeaponClass::ReturnWeaponObject()
{
	return weaponObject;
}