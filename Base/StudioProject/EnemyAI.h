#ifndef ENEMY_AI_H
#define ENEMY_AI_H

#include "../Core/GameObject.h"
#include "GOList.h"
#include "AIBehaviour.h"

class EnemyAI
{
public:
	float m_fwalkTime;
	bool m_bwalk01;
	bool m_bwalk02;
	Vector3 m_v3playerPos;


	GOList &gl = GOList::GetInstance();

	GameObject* enemy;
	AIBehaviour behaviour;

	AITree::Sequence *root;
	AITree::Sequence *sequence1;
	AITree::Selector *selector1;
	AIBehaviour *aiStatus;
	CheckPlayerInSightTask *checkSight;
	ApproachPlayerTask *approachPlayer;

	EnemyAI();
	~EnemyAI();
	void Init();
	void Update(double dt);
	void CheckPlayerInSight(GameObject* _player);

};

#endif