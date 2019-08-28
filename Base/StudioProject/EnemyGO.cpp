#include "EnemyGO.h"

EnemyGO::EnemyGO()
	: m_bactivatePathFind(false)
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
	if (CheckRangeNode != NULL)
	{
		delete CheckRangeNode;
		CheckRangeNode == NULL;
	}
	if (ApproachPlayerNode != NULL)
	{
		delete ApproachPlayerNode;
		ApproachPlayerNode == NULL;
	}
	if (idle != NULL)
	{
		delete idle;
		idle == NULL;
	}
}

void EnemyGO::Init()
{
	aiStatus = new AIBehaviour(false, 0);
	aiStatus->state = AIBehaviour::IDLE;

	root = new AITree::Sequence;
	sequence1 = new AITree::Sequence;
	sequence2 = new AITree::Sequence;
	sequence3 = new AITree::Sequence;
	sequence4 = new AITree::Sequence;
	selector1 = new AITree::Selector;
	selector2 = new AITree::Selector;
	selector3 = new AITree::Selector;
	selector4 = new AITree::Selector;
	selector5 = new AITree::Selector;
	CheckRangeNode = new CheckPlayerInRangeTask(aiStatus);
	ApproachPlayerNode = new ApproachPlayerTask(aiStatus);
	ShootPlayerNode = new ShootPlayerTask(aiStatus);
	ifGunHeld = new CheckIfGunHeldTask(aiStatus, false);
	DetectBulletNode = new CheckOncomingBulletTask(aiStatus);
	ChasePlayerNode = new ChasePlayerTask(aiStatus);
	SearchGunNode = new SearchNeabyGunTask(aiStatus);
	idle = new IdleTask(aiStatus);

	// If player in range
	root->addChild(selector1);


	selector1->addChild(sequence1);
	sequence1->addChild(CheckRangeNode);
	sequence1->addChild(selector2);

	selector2->addChild(DetectBulletNode);
	selector2->addChild(selector3);

	selector3->addChild(ApproachPlayerNode);
	selector3->addChild(selector4);

	selector4->addChild(sequence3);
	selector4->addChild(selector5);

	sequence3->addChild(ifGunHeld);
	sequence3->addChild(ShootPlayerNode);

	selector5->addChild(sequence4);
	selector5->addChild(ChasePlayerNode);

	sequence4->addChild(SearchGunNode);



	selector1->addChild(sequence2);


	m_bdoOnce = false;
	m_fROF = Math::RandFloatMinMax(0.f, 2.f);

	GunOnHand = NULL;

	m_bactivatePathFind = false;
}


void EnemyGO::Init(std::vector <PathNode *> &_pn)
{
	m_pnList = _pn;
	parent = NULL;
	destination = NULL;
	currentNode = NULL;

	aiStatus = new AIBehaviour(false, 0);
	aiStatus->state = AIBehaviour::IDLE;

	root = new AITree::Sequence;
	sequence1 = new AITree::Sequence;
	sequence2 = new AITree::Sequence;
	sequence3 = new AITree::Sequence;
	sequence4 = new AITree::Sequence;
	selector1 = new AITree::Selector;
	selector2 = new AITree::Selector;
	selector3 = new AITree::Selector;
	selector4 = new AITree::Selector;
	selector5 = new AITree::Selector;
	CheckRangeNode = new CheckPlayerInRangeTask(aiStatus);
	ApproachPlayerNode = new ApproachPlayerTask(aiStatus);
	ShootPlayerNode = new ShootPlayerTask(aiStatus);
	ifGunHeld = new CheckIfGunHeldTask(aiStatus, false);
	DetectBulletNode = new CheckOncomingBulletTask(aiStatus);
	ChasePlayerNode = new ChasePlayerTask(aiStatus);
	SearchGunNode = new SearchNeabyGunTask(aiStatus);
	idle = new IdleTask(aiStatus);

	// If player in range
	root->addChild(selector1);


	selector1->addChild(sequence1);
	sequence1->addChild(CheckRangeNode);
	sequence1->addChild(selector2);

	selector2->addChild(DetectBulletNode);
	selector2->addChild(selector3);

	selector3->addChild(ApproachPlayerNode);
	selector3->addChild(selector4);

	selector4->addChild(sequence3);
	selector4->addChild(selector5);

	sequence3->addChild(ifGunHeld);
	sequence3->addChild(ShootPlayerNode);

	selector5->addChild(sequence4);
	selector5->addChild(ChasePlayerNode);

	sequence4->addChild(SearchGunNode);

	selector1->addChild(sequence2);


	m_bdoOnce = false;
	m_fROF = Math::RandFloatMinMax(0.f, 2.f);

	GunOnHand = NULL;

	m_bactivatePathFind = true;
}

bool EnemyGO::Constrain(Collider box, double dt)
{
	GOList &gl = GOList::GetInstance();

	for (auto go : gl.m_goList)
	{
		if (go->type == GameObject::GO_ENEMY && static_cast<EnemyGO*>(go) != this)
		{
			Collider box2 = go->obb;
			//Vector3 m_v3playerPos = gl.FetchGO(GameObject::GO_PLAYER)->transform.position;
			//Vector3 m_v3dir1 = (m_v3playerPos - go->transform.position).Normalized();
			//box2.UpdatePos((go->transform.position+(Vector3(m_v3dir1.x, 0, m_v3dir1.z) * 20 * dt) * 20));
			if (box.GetCollision(box2))
			{
				return true;
			}
		}

	}
	return false;
}

void EnemyGO::Update(double dt, PlayerGO* _player)
{
	m_fROF += dt;
	obb.RotateAxis(Math::RadianToDegree(atan2(m_v3dir.x, m_v3dir.z)), Vector3(0, 1, 0));
	aiStatus->m_v3aiPosition = transform.position;
	aiStatus->m_fdistanceToPlayer = (transform.position - _player->transform.position).Length();

	if (m_bactivatePathFind)
	{
		if (parent == NULL)
		{
			for (auto pn : m_pnList)
			{
				if (obb.GetCollision(pn->obb) && (pn->transform.position - transform.position).Length() < 15)
				{
					pn->m_inodeStatus = 2;
					parent = pn;
					if (currentNode == NULL)
						currentNode = parent;
					break;
				}
			}
		}

		if (destination == NULL)
		{
			for (auto pn : m_pnList)
			{
				if (_player->obb.GetCollision(pn->obb) && (pn->transform.position - _player->transform.position).Length() < 15)
				{
					if (destination != NULL)
						destination->m_inodeStatus = 0;
					destination = pn;
					break;
				}
			}
		}

		if (currentNode != NULL && destination != NULL && (destination->transform.position - currentNode->transform.position).Length() > 15)
		{
			float shortestDist = 1000;
			PathNode *storeNode;
			for (auto pn : m_pnList)
			{
				float newDist = (pn->transform.position - destination->transform.position).Length();
				if (newDist < shortestDist && (pn->transform.position - currentNode->transform.position).Length() < 15 && pn->m_inodeStatus != 1 && pn != currentNode)
				{
					storeNode = pn;
					shortestDist = newDist;
				}
			}
			currentNode = storeNode;
			currentNode->m_inodeStatus = 2;
			m_pathList.push_back(currentNode);
		}

		static int x = 0;
		if (ChasePlayerNode->GetApproachBool() && !SearchGunNode->ReturnGunFound() && x < m_pathList.size())
		{

			m_v3dir = (m_pathList[x]->transform.position - transform.position).Normalized();

			aiStatus->state = AIBehaviour::WALK;

			if (m_bstartwalk)
				transform.position += Vector3(m_v3dir.x, 0, m_v3dir.z) * 20 * dt;

			//if (!Constrain((transform.position + (Vector3(m_v3dir.x, 0, m_v3dir.z) * 20 * dt) * 20), obb, dt))
			//	transform.position += Vector3(m_v3dir.x, 0, m_v3dir.z) * 20 * dt;
			//else
			//	transform.position += Vector3(Vector3(-m_v3dir.x, 0, m_v3dir.z) * 20 * dt);
				
			if ((transform.position - m_pathList[x]->transform.position).Length() < 15)
				x++;
		}

		if (destination != NULL)
		{
			if ((_player->transform.position - destination->transform.position).Length() > 25)
			{
				for (auto pn : m_pathList)
				{
					pn->m_inodeStatus = 0;
				}
				parent = NULL;
				destination = NULL;
				m_pathList.clear();
				currentNode = parent;
				x = 0;
			}
		}
	}
	else
	{
		if (ApproachPlayerNode->GetApproachBool())
		{
			m_v3dir = (_player->transform.position - transform.position).Normalized();

			aiStatus->state = AIBehaviour::WALK;

			for (auto go : gl.m_goList)
			{
				if (go->type == GameObject::GO_ENEMY && static_cast<EnemyGO*>(go) != this)
				{
					static_cast<EnemyGO*>(go)->m_bstartwalk = false;
				}
			}

			if (m_bstartwalk)
				transform.position += Vector3(m_v3dir.x, 0, m_v3dir.z) * 20 * dt;

		}
		if (ChasePlayerNode->GetApproachBool() && !SearchGunNode->ReturnGunFound())
		{
			m_v3dir = (_player->transform.position - transform.position).Normalized();

			aiStatus->state = AIBehaviour::WALK;

			for (auto go : gl.m_goList)
			{
				if (go->type == GameObject::GO_ENEMY && static_cast<EnemyGO*>(go) != this)
					if (obb.GetCollision(go->obb))
					{
						static_cast<EnemyGO*>(go)->m_bstartwalk = false;
						break;
					}
					else
						static_cast<EnemyGO*>(go)->m_bstartwalk = true;
			}

			if (m_bstartwalk)
				transform.position += Vector3(m_v3dir.x, 0, m_v3dir.z) * 20 * dt;
		}
	}


	if (GunOnHand != NULL)
	{
		ifGunHeld->SetGunHeld(true);
	}

	if (ShootPlayerNode->GetShootStatus() && m_fROF >= 2)
	{
		m_v3dir = (_player->transform.position - transform.position).Normalized();

		GameObject *go = gl.FetchGO();
		go->SetActive(true);
		go->SetStatic(false);
		go->m_bGravity = false;
		go->type = GameObject::GO_EBULLET;
		go->transform.position = Vector3(transform.position.x, transform.position.y + 10, transform.position.z);
		go->transform.scale.Set(0.3, 0.3, 0.3);
		go->m_v3dir = m_v3dir;
		go->m_v3vel = go->m_v3dir * 200.f;

		go->obb.SetScale(Vector3(0.15, 0.15, 0.15));
		go->obb.UpdateAxis(Vector3(1, 0, 0), Vector3(0, 0, 1));
		go->obb.pos = go->transform.position;
		go->obb.RotateAxis(0, Vector3(0, 1, 0));
		m_fROF = Math::RandFloatMinMax(0.f, 1.f);
	}

	if (SearchGunNode->ReturnGunFound())
	{
		m_v3dir = (Vector3(SearchGunNode->ReturnPistol()->transform.position.x - transform.position.x, 19, SearchGunNode->ReturnPistol()->transform.position.z - transform.position.z)).Normalized();

		transform.position += Vector3(m_v3dir.x, 0, m_v3dir.z) * 20 * dt;
	}

	if (aiStatus->m_bdodge)
	{
		int decision = rand() % 3;
		if (decision == 1)
			transform.position += DetectBulletNode->ReturnBullet()->m_v3dir.Cross(Vector3(0, 1, 0)) * 30 * dt;
		if (decision == 2)
			transform.position -= DetectBulletNode->ReturnBullet()->m_v3dir.Cross(Vector3(0, 1, 0)) * 30 * dt;
	}

	if (aiStatus->state == AIBehaviour::WALK)
	{
		m_fwalkTime += dt;

		aiStatus->m_bstartWalk01 = false;
		aiStatus->m_bstartWalk02 = true;

		//if (m_fwalkTime >= 4.f && aiStatus->m_bstartWalk01)
		//{
		//	aiStatus->m_bstartWalk01 = false;
		//	aiStatus->m_bstartWalk02 = true;
		//	m_fwalkTime = 0.f;
		//}
		//else if (m_fwalkTime >= 4.f && aiStatus->m_bstartWalk02)
		//{
		//	aiStatus->m_bstartWalk01 = true;
		//	aiStatus->m_bstartWalk02 = false;
		//	m_fwalkTime = 0.f;
		//}
	}

	//if (approachPlayer->GetApproachBool())
	//{

	//	aiStatus->state = AIBehaviour::WALK;

	//	if (!m_bdoOnce)
	//	{
	//		aiStatus->m_bstartWalk01 = true;
	//		m_bdoOnce = true;
	//	}

	//	if (!Constrain((transform.position+(Vector3(m_v3dir.x, 0, m_v3dir.z) * 20 * dt)* 20), obb, dt))
	//		transform.position += Vector3(m_v3dir.x, 0, m_v3dir.z) * 20 * dt;
	//	else
	//		transform.position += Vector3(Vector3(-m_v3dir.x, 0, m_v3dir.z) * 20 * dt);
	//}
	//else
	//{
	//	aiStatus->state = AIBehaviour::IDLE;
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

	//std::cout << aiStatus->m_fdistanceToPlayer << std::endl;

	while (!root->run())
		std::cout << "======Break======" << std::endl;
}

void EnemyGO::SetGunOnHand(PistolGO* _gunOnHand)
{
	GunOnHand = _gunOnHand;
}

void EnemyGO::SetPathFind(bool _pathfind)
{
	m_bactivatePathFind = _pathfind;
}