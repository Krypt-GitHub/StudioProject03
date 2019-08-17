#include "EnemyGO.h"
#include "PlayerGO.h"


EnemyGO::EnemyGO()
{
}


EnemyGO::~EnemyGO()
{
	if (root != NULL)
	{
		delete root;
		root == NULL;
	}
	if (sequence1 != NULL)
	{
		delete sequence1;
		sequence1 == NULL;
	}
	if (sequence2 != NULL)
	{
		delete sequence2;
		sequence2 == NULL;
	}
	if (selector1 != NULL)
	{
		delete selector1;
		selector1 == NULL;
	}
	if (aiStatus != NULL)
	{
		delete aiStatus;
		aiStatus == NULL;
	}
	if (checkSight != NULL)
	{
		delete checkSight;
		checkSight == NULL;
	}
	if (approachPlayer != NULL)
	{
		delete approachPlayer;
		approachPlayer == NULL;
	}
	if (idle != NULL)
	{
		delete idle;
		idle == NULL;
	}
}

void EnemyGO::Init()
{
	root = new AITree::Sequence;
	sequence1 = new AITree::Sequence;
	selector1 = new AITree::Selector;
	sequence2 = new AITree::Sequence;
	aiStatus = new AIBehaviour(false, 0);
	aiStatus->state = AIBehaviour::IDLE;
	checkSight = new CheckPlayerInSightTask(aiStatus);
	approachPlayer = new ApproachPlayerTask(aiStatus, false);
	idle = new IdleTask(aiStatus);

	root->addChild(selector1);

	selector1->addChild(sequence1);
	selector1->addChild(sequence2);

	sequence1->addChild(checkSight);
	sequence1->addChild(approachPlayer);

	sequence2->addChild(idle);

	//GameObject* test;
	//test = gl.FetchGO();


	//m_v3playerPos = gl.FetchGO(GameObject::GO_PLAYER)->transform.position;
}

void EnemyGO::Update(double dt)
{

	//if (aiStatus->state == AIBehaviour::WALK)
	//{
	//	if (m_bdoOnce)
	//		aiStatus->m_bstartWalk01 = true;

	//	m_v3dir = (m_v3playerPos - transform.position).Normalize();
	//	transform.position += Vector3(m_v3dir.x, 0, m_v3dir.z) * 20 * dt;
	//}

	//if (aiStatus->m_bstartWalk01)
	//{
	//	m_fwalkTime += dt;
	//	if (m_fwalkTime >= 0.4f)
	//	{
	//		aiStatus->m_bstartWalk01 = false;
	//		aiStatus->m_bstartWalk02 = true;
	//		m_fwalkTime = 0.f;
	//	}
	//}

	//if (aiStatus->m_bstartWalk02)
	//{
	//	m_fwalkTime += dt;
	//	if (m_fwalkTime >= 0.4f)
	//	{
	//		aiStatus->m_bstartWalk02 = false;
	//		aiStatus->m_bstartWalk01 = true;
	//		m_fwalkTime = 0.f;
	//	}
	//}

	//aiStatus->m_fdistanceToPlayer = (transform.position - m_v3playerPos).Length();
	//std::cout << aiStatus->m_fdistanceToPlayer << std::endl;

	//while (!root->run())
	//	std::cout << "gay" << std::endl;
}
