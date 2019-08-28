#ifndef PISTOL_GO_H
#define PISTOL_GO_H

#include "WeaponBase.h"
#include "../Core/GameObject.h"

class PistolGO : public WeaponBase
{
public:
	PistolGO();
	virtual ~PistolGO();

	void Init();
	void Update(double dt);

	GameObject *attachedGO;
	float m_frotation;
	bool m_bCanFire;
	bool m_bRotateCrosshair;
};

#endif
