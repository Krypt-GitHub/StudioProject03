#ifndef ENEMY_GO_H
#define ENEMY_GO_H

#include "../Core/GameObject.h"
#include "GOList.h"
#include "AIBehaviour.h"
#include "PistolGO.h"

class EnemyGO : public GameObject
{
private:
	float m_fwalkTime;
	bool m_bdoOnce;
	float m_fROF;
public:
	EnemyGO();
	virtual ~EnemyGO();

	GOList &gl = GOList::GetInstance();

	Vector3 m_v3playerPos;

	AITree::Sequence *root;
	AITree::Sequence *sequence1;
	AITree::Selector *selector1;
	AITree::Sequence *sequence2;
	AITree::Selector *selector2;

	AIBehaviour *aiStatus;
	CheckPlayerInRangeTask *checkRange;
	ApproachPlayerTask *approachPlayer;
	ShootPlayerTask *shootPlayer;
	IdleTask *idle;

	PistolGO *gunHeld;

	void Init();
	void Update(double dt);
};

#endif