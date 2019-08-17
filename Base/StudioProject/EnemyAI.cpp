#include "EnemyAI.h"

EnemyAI::EnemyAI()
	: m_fwalkTime(0.f)
	, m_bdoOnce(false)
{
}

EnemyAI::~EnemyAI()
{
}

void EnemyAI::Init()
{
	//enemy = new GameObject;
	//enemy->SetActive(true);
	//enemy->SetStatic(false);
	//enemy->type = GameObject::GO_ENEMY;
	//enemy->transform.SetTransform(Vector3(-10, 0, -10), Vector3(2, 2, 2), 0);


	//gl.m_goList.push_back(enemy);

	//root = new AITree::Sequence;
	//sequence1 = new AITree::Sequence;
	//selector1 = new AITree::Selector;
	//sequence2 = new AITree::Sequence;
	//aiStatus = new AIBehaviour(false, 0);
	//aiStatus->state = AIBehaviour::IDLE;
	//checkSight = new CheckPlayerInSightTask(aiStatus);
	//approachPlayer = new ApproachPlayerTask(aiStatus, false);
	//idle = new IdleTask(aiStatus);

	//root->addChild(selector1);

	//selector1->addChild(sequence1);
	//selector1->addChild(sequence2);

	//sequence1->addChild(checkSight);
	//sequence1->addChild(approachPlayer);

	//sequence2->addChild(idle);
}

void EnemyAI::Update(double dt)
{
	m_v3playerPos = gl.FetchGO(GameObject::GO_PLAYER)->transform.position;

	if (aiStatus->state == AIBehaviour::WALK)
	{
		if (m_bdoOnce)
			aiStatus->m_bstartWalk01 = true;
		
		enemy->m_v3dir = (m_v3playerPos - enemy->transform.position).Normalize();
		enemy->transform.position += Vector3(enemy->m_v3dir.x, 0, enemy->m_v3dir.z) * 20 * dt;
	}

	if (aiStatus->m_bstartWalk01)
	{
		m_fwalkTime += dt;
		if (m_fwalkTime >= 0.4f)
		{
			aiStatus->m_bstartWalk01 = false;
			aiStatus->m_bstartWalk02 = true;
			m_fwalkTime = 0.f;
		}
	}

	if (aiStatus->m_bstartWalk02)
	{
		m_fwalkTime += dt;
		if (m_fwalkTime >= 0.4f)
		{
			aiStatus->m_bstartWalk02 = false;
			aiStatus->m_bstartWalk01 = true;
			m_fwalkTime = 0.f;
		}
	}

	aiStatus->m_fdistanceToPlayer = (enemy->transform.position - m_v3playerPos).Length();
	//std::cout << aiStatus->m_fdistanceToPlayer << std::endl;

	while (!root->run())
		std::cout << "gay" << std::endl;
}

//void EnemyAI::CheckPlayerInSight(GameObject* _player)
//{
//	if ((enemy->transform.position - _player->transform.position).Length() <= 100.f)
//	{
//		aiState.ai.playerInSight = true;
//	}
//	else
//		aiState.ai.playerInSight = false;
//}
