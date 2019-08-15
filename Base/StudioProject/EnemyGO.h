#ifndef ENEMY_GO_H
#define ENEMY_GO_H

#include "../Core/GameObject.h"
#include "GOList.h"
#include "AIBehaviour.h"

class EnemyGO : public GameObject
{
private:
	float m_fwalkTime;
	bool m_bdoOnce;
public:
	EnemyGO();
	~EnemyGO();

	GOList &gl = GOList::GetInstance();

	Vector3 m_v3playerPos;

	AITree::Sequence *root;
	AITree::Sequence *sequence1;
	AITree::Selector *selector1;
	AITree::Sequence *sequence2;
	AIBehaviour *aiStatus;
	CheckPlayerInSightTask *checkSight;
	ApproachPlayerTask *approachPlayer;
	IdleTask *idle;

	void Init();
	void Update(double dt);
};

#endif