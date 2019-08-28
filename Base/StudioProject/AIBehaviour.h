#ifndef AI_BEHAVIOUR_H
#define AI_BEHAVIOUR_H

#include "BehaviourNode.h"
#include "Vector3.h"
#include "GOList.h"
#include "PistolGO.h"

class AIBehaviour
{
public:
	enum AI_STATE
	{
		IDLE = 0,
		WALK,
		SHOOT,
		NUM_STATE,
	};

	AIBehaviour();
	~AIBehaviour();

	AIBehaviour(bool _playerInSight, float _distanceToPlayer);

	AI_STATE state;
	//bool m_bplayerInSight;
	//bool m_bstartShooting;
	float m_fdistanceToPlayer;
	bool m_bstartWalk01;
	bool m_bstartWalk02;
	Vector3 m_v3aiPosition;
	bool m_bdodge;
};

class CheckPlayerInRangeTask : public AITree::Node
{
private:
	AIBehaviour* status;
public:
	CheckPlayerInRangeTask(AIBehaviour* status) : status(status) {}
	virtual bool run() override
	{
		if (status->m_fdistanceToPlayer < 1000)
		{
			return true;
		}
		return false;
	}
};

class IdleTask : public AITree::Node
{
private:
	AIBehaviour *status;
public:
	IdleTask(AIBehaviour* status) : status(status) {}
	virtual bool run() override
	{
		if (status->m_fdistanceToPlayer > 1000)
		{
			//std::cout << "AI on idle" << std::endl;
			return true;
		}
		return false;
	}
};

class ApproachPlayerTask : public AITree::Node
{
private:
	AIBehaviour* status;
	bool m_bapproachPlayer;
public:
	ApproachPlayerTask(AIBehaviour *status) : status(status), m_bapproachPlayer(false) {}
	virtual bool run() override
	{
		if (status->m_fdistanceToPlayer >= 700)
		{
			m_bapproachPlayer = true;
			return true;
		}
		else
		{
			m_bapproachPlayer = false;
			return false;
		}
	}
	bool GetApproachBool()
	{
		return m_bapproachPlayer;
	}
};

class CheckOncomingBulletTask : public AITree::Node
{
private:
	AIBehaviour* status;
	GameObject* bullet;
public:
	CheckOncomingBulletTask(AIBehaviour *status) : status(status) {}
	virtual bool run() override
	{
		GOList &gl = GOList::GetInstance();
		for (auto go : gl.m_goList)
		{
			if (go->GetActive())
			{
				if (go->type == GameObject::GO_PBULLET && (status->m_v3aiPosition - go->transform.position).Length() <= 40)
				{
					status->m_bdodge = true;
					bullet = go;
					return true;
				}
			}
		}
		status->m_bdodge = false;
		return false;
	}
	GameObject* ReturnBullet()
	{
		return bullet;
	}
};

class CheckIfGunHeldTask : public AITree::Node
{
private:
	AIBehaviour* status;
	bool gunHeld;
public:
	CheckIfGunHeldTask(AIBehaviour *status, bool _gunHeld) : status(status), gunHeld(false) {}
	virtual bool run() override
	{
		if (gunHeld)
		{
			return true;
		}
		else
			return false;
	}
	void SetGunHeld(bool _gunHeld)
	{
		gunHeld = _gunHeld;
	}
};

class ChasePlayerTask : public AITree::Node 
{
private:
	AIBehaviour* status;
	bool m_bapproachPlayer;
public:
	ChasePlayerTask(AIBehaviour *status) : status(status), m_bapproachPlayer(false) {}
	virtual bool run() override
	{
		if (status->m_fdistanceToPlayer >= 50)
		{
			m_bapproachPlayer = true;
			return true;
		}
		else
		{
			m_bapproachPlayer = false;
			return false;
		}
	}
	bool GetApproachBool()
	{
		return m_bapproachPlayer;
	}
};

class SearchNearbyGunTask : public AITree::Node
{
private:
	AIBehaviour* status;	
	GameObject* pistol;
	bool m_bgunFound;
public:
	SearchNearbyGunTask(AIBehaviour *status) : status(status), m_bgunFound(false) {}
	virtual bool run() override
	{
		GOList &gl = GOList::GetInstance();
		for (auto go : gl.m_goList)
		{
			if (go->GetActive())
			{
				if (go->type == GameObject::GO_PISTOL && (status->m_v3aiPosition - go->transform.position).Length() <= 100 /*&& !static_cast<PistolGO*>(go)->GetPickUp()*/)
				{
					pistol = go;
					m_bgunFound = true;
					return true;
				}
			}
		}
		m_bgunFound = false;
		return false;
	}
	GameObject* ReturnPistol()
	{
		return pistol;
	}
	bool ReturnGunFound()
	{
		return m_bgunFound;
	}
};

class ShootPlayerTask : public AITree::Node
{
private:
	AIBehaviour* status;
	bool m_bstartShooting;
public:
	ShootPlayerTask(AIBehaviour *status) : status(status), m_bstartShooting(false) {}
	virtual bool run() override
	{
		if (status->m_fdistanceToPlayer <= 700)
		{
			m_bstartShooting = true;
			return true;
		}
		else
		{
			m_bstartShooting = false;
			return false;
		}
	}
	bool GetShootStatus()
	{
		return m_bstartShooting;
	}
};


#endif
