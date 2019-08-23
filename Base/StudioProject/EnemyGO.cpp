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
	if (selector2 != NULL)
	{
		delete selector2;
		selector2 == NULL;
	}
	if (aiStatus != NULL)
	{
		delete aiStatus;
		aiStatus == NULL;
	}
	if (checkRange != NULL)
	{
		delete checkRange;
		checkRange == NULL;
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
	selector2 = new AITree::Selector;
	aiStatus = new AIBehaviour(false, 0);
	aiStatus->state = AIBehaviour::IDLE;
	checkRange = new CheckPlayerInRangeTask(aiStatus);
	approachPlayer = new ApproachPlayerTask(aiStatus);
	shootPlayer = new ShootPlayerTask(aiStatus);
	idle = new IdleTask(aiStatus);

	root->addChild(selector1);

	selector1->addChild(sequence1);
	selector1->addChild(sequence2);

	sequence1->addChild(checkRange);
	sequence1->addChild(selector2);
	selector2->addChild(approachPlayer);
	selector2->addChild(shootPlayer);

	sequence2->addChild(idle);

	m_bdoOnce = false;
	m_fROF = Math::RandFloatMinMax(0.f, 2.f);
}

bool EnemyGO::Constrain(Vector3 futurepos, Collider box, double dt)
{
	GOList &gl = GOList::GetInstance();

	box.UpdatePos(futurepos);
	//box.SetScale(Vector3( box.Half_size.y));
	for (auto go : gl.m_goList)
	{
		if (go->type == GameObject::GO_ENEMY && static_cast<EnemyGO*>(go) != this)
		{
			Collider box2 = go->obb;
			Vector3 m_v3playerPos = gl.FetchGO(GameObject::GO_PLAYER)->transform.position;
			Vector3 m_v3dir1 = (m_v3playerPos - go->transform.position).Normalized();
			box2.UpdatePos((go->transform.position+(Vector3(m_v3dir1.x, 0, m_v3dir1.z) * 20 * dt) * 20));
			if (box.GetCollision(box2))
			{
				return true;
			}
		}
		
	}
	return false;
}

void EnemyGO::Update(double dt)
{
	m_fROF += dt;
	Vector3 m_v3playerPos = gl.FetchGO(GameObject::GO_PLAYER)->transform.position;
	m_v3dir = (m_v3playerPos - transform.position).Normalized();
	obb.RotateAxis(Math::RadianToDegree(atan2(m_v3dir.x, m_v3dir.z)), Vector3(0, 1, 0));

	if (approachPlayer->GetApproachBool())
	{
		aiStatus->state = AIBehaviour::WALK;

		if (!m_bdoOnce)
		{
			aiStatus->m_bstartWalk01 = true;
			m_bdoOnce = true;
		}

		if (!Constrain((transform.position+(Vector3(m_v3dir.x, 0, m_v3dir.z) * 20 * dt)* 20), obb, dt))
			transform.position += Vector3(m_v3dir.x, 0, m_v3dir.z) * 20 * dt;
		else
			transform.position += Vector3(Vector3(-m_v3dir.x, 0, m_v3dir.z) * 20 * dt);
	}
	else
	{
		aiStatus->state = AIBehaviour::IDLE;
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

	if (shootPlayer->GetShootStatus() && m_fROF >= 2)
	{
		GameObject *go = gl.FetchGO();
		go->type = GameObject::GO_BULLET;
		go->SetActive(true);
		go->m_bGravity = false;
		go->transform.position = Vector3(transform.position.x, transform.position.y + 10, transform.position.z + 20);
		go->transform.scale.Set(0.3, 0.3, 0.3);
		go->m_v3dir = m_v3dir;
		go->m_v3vel = go->m_v3dir * 200.f;

		go->obb.SetScale(Vector3(0.15, 0.15, 0.15));
		go->obb.UpdateAxis(Vector3(1, 0, 0), Vector3(0, 0, 1));
		go->obb.pos = transform.position;
		go->obb.RotateAxis(0, Vector3(0, 1, 0));
		m_fROF = Math::RandFloatMinMax(0.f, 1.f);
	}


	aiStatus->m_fdistanceToPlayer = (transform.position - m_v3playerPos).Length();

	std::cout << aiStatus->m_fdistanceToPlayer << std::endl;

	while (!root->run())
		std::cout << "======Break======" << std::endl;
}
