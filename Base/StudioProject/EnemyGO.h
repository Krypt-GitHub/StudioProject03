#ifndef ENEMY_GO_H
#define ENEMY_GO_H

#include "../Core/GameObject.h"
#include "GOList.h"
#include "AIBehaviour.h"
#include "PistolGO.h"
#include "PlayerGO.h"


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

	AITree::Sequence *root;
	AITree::Sequence *sequence1;
	AITree::Sequence *sequence2;
	AITree::Sequence *sequence3;
	AITree::Sequence *sequence4;
	AITree::Selector *selector1;
	AITree::Selector *selector2;
	AITree::Selector *selector3;
	AITree::Selector *selector4;
	AITree::Selector *selector5;

	AIBehaviour *aiStatus;
	CheckPlayerInRangeTask *CheckRangeNode;
	ApproachPlayerTask *ApproachPlayerNode;
	ShootPlayerTask *ShootPlayerNode;
	IdleTask *idle;
	CheckIfGunHeldTask *ifGunHeld;
	CheckOncomingBulletTask *DetectBulletNode;
	ChasePlayerTask *ChasePlayerNode;
	SearchNeabyGunTask *SearchGunNode;

	PistolGO *GunOnHand;

	void Init();
	bool Constrain(Vector3 futurepos, Collider box, double dt);
	void Update(double dt, PlayerGO * _player);
	void SetGunOnHand(PistolGO * _gunOnHand);
	bool Constrain(Vector3 futurepos, Collider box);
};

#endif