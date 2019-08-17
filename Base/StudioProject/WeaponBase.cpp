#include "WeaponBase.h"



WeaponBase::WeaponBase()
	: m_bisPickUp(false)
	, m_ifireRate(10.f)
	, m_freloadSpeed(5.f)
	, m_imaxClip(10)
{
}


WeaponBase::~WeaponBase()
{
}

void WeaponBase::SetPickUp(bool _isPickUp)
{
	m_bisPickUp = _isPickUp;
}

bool WeaponBase::GetPickUp()
{
	return m_bisPickUp;
}
