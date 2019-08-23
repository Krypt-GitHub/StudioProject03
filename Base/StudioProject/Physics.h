#ifndef PHYSICS_H
#define PHYSICS_H

#include "GOList.h"

class Physics
{
private:
	int EnemyCount;
public:
	GOList &gl = GOList::GetInstance();

	Physics();
	~Physics();

	void UpdateGO(double dt);
	void CollisionResponse(GameObject *go1, GameObject *go2);

	int GetEnemyCount();
	void SetEnemyCount(int _count);
};

#endif