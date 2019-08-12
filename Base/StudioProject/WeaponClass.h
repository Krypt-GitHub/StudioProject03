#ifndef WEAPON_CLASS_H
#define WEAPON_CLASS_H

#include "../Core/GameObject.h"
#include <vector>
#include "GOList.h"

class WeaponClass
{
public:
	// Members
	GameObject* weaponObject;


	// Functions
	WeaponClass();
	~WeaponClass();

	void Init(const std::string _name, int _fireRate, float _reloadSpeed, int _maxClip);
	void Update(double dt, Vector3 _dir);
	void SetPickUp(bool _isPickUp);
	bool GetPickUp();
	void UpdatePosition(Vector3 _playerPosition);
	int GetCurrClip();

	GameObject* ReturnWeaponObject();
private:
	// Members
	bool m_bisPickUp;
	int m_imaxClip;
	int m_icurClip;
	int m_ifireRate;
	float m_freloadSpeed;
	Vector3 m_v3storePosition;

	GOList &gl = GOList::GetInstance();
};

#endif