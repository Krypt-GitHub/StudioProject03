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
};

#endif
