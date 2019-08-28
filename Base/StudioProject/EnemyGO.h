#ifndef ENEMY_GO_H
#define ENEMY_GO_H

#include "../Core/GameObject.h"
#include "GOList.h"
#include "AIBehaviour.h"
#include "PistolGO.h"
#include "PlayerGO.h"
#include "PathNode.h"

class EnemyGO : public GameObject
{
private:
	float m_fwalkTime;
	bool m_bdoOnce;
	float m_fROF;
	bool m_bactivatePathFind;
	std::vector <PathNode*> m_pnList;
	std::vector <PathNode*> m_pathList;
	PathNode* parent;
	PathNode* destination;
	PathNode *currentNode;
public:

	bool m_bstartwalk;

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
	SearchNearbyGunTask *SearchGunNode;

	PistolGO *GunOnHand;

	void Init();
	void Init(std::vector<PathNode*>& _pn);
	bool Constrain(Collider box, double dt);
	void Update(double dt, PlayerGO * _player);
	void SetGunOnHand(PistolGO * _gunOnHand);
	void SetPathFind(bool _pathfind);
};

#endif