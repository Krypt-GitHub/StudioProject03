#include "EnemyAI.h"

EnemyAI::EnemyAI()
	: m_fwalkTime(0.f)
	, m_bwalk01(false)
	, m_bwalk02(false)
{
}

EnemyAI::~EnemyAI()
{
}

void EnemyAI::Init()
{
	enemy = new GameObject;
	enemy->SetActive(true);
	enemy->SetStatic(false);
	enemy->type = GameObject::GO_ENEMY;
	enemy->transform.position.Set(-10, 0, -10);

	behaviour.state = AIBehaviour::IDLE;

	gl.m_goList.push_back(enemy);

	root = new AITree::Sequence;
	sequence1 = new AITree::Sequence;
	selector1 = new AITree::Selector;
	aiStatus = new AIBehaviour(false, 0);
	checkSight = new CheckPlayerInSightTask(aiStatus);
	approachPlayer = new ApproachPlayerTask(aiStatus, false);

	root->addChild(selector1);

	selector1->addChild(checkSight);
	selector1->addChild(sequence1);

	sequence1->addChild(approachPlayer);
}

void EnemyAI::Update(double dt)
{
	m_v3playerPos = gl.FetchGO(GameObject::GO_PLAYER)->transform.position;

	if (m_bwalk01)
	{
		m_fwalkTime += dt;
		if (m_fwalkTime >= 0.4f)
		{
			m_bwalk01 = false;
			m_bwalk02 = true;
			m_fwalkTime = 0.f;
		}
	}

	if (m_bwalk02)
	{
		m_fwalkTime += dt;
		if (m_fwalkTime >= 0.4f)
		{
			m_bwalk02 = false;
			m_bwalk01 = true;
			m_fwalkTime = 0.f;
		}
	}

	aiStatus->m_fdistanceToPlayer = (enemy->transform.position - m_v3playerPos).Length();
	std::cout << aiStatus->m_fdistanceToPlayer << std::endl;

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
