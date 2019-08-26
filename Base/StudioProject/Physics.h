#ifndef PHYSICS_H
#define PHYSICS_H

#include "GOList.h"
#include "PlayerGO.h"

class Physics
{
private:
	int EnemyCount;
public:
	GOList &gl = GOList::GetInstance();

	Physics();
	~Physics();

	void UpdateGO(double dt, PlayerGO* _player);
	void CollisionResponse(GameObject *go1, GameObject *go2);

	int GetEnemyCount();
	void SetEnemyCount(int _count);
};

#endif