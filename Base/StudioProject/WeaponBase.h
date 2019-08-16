#ifndef WEAPON_BASE_H
#define WEAPON_BASE_H

#include "Vector3.h"

class WeaponBase
{
private:
	bool m_bisPickUp;
	int m_imaxClip;
	int m_icurClip;
	int m_ifireRate;
	float m_freloadSpeed;
	Vector3 m_v3storePosition;
public:
	WeaponBase();
	~WeaponBase();
};

#endif