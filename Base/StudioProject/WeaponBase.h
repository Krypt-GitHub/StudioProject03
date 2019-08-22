#ifndef WEAPON_BASE_H
#define WEAPON_BASE_H

#include "Vector3.h"
#include "../Core/GameObject.h"

class WeaponBase : public GameObject
{
protected:
	bool m_bisPickUp;
	int m_imaxClip;
	int m_icurClip;
	int m_ifireRate;
	float m_freloadSpeed;
	Vector3 m_v3storePos;
	Vector3 m_v3storeDir;
	GameObject *m_goStorePlayer;
public:
	WeaponBase();
	~WeaponBase();

	void SetPickUp(bool _isPickUp);
	bool GetPickUp();
	int GetCurrClip();
};

#endif