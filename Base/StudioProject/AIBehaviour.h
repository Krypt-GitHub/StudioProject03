#ifndef AI_BEHAVIOUR_H
#define AI_BEHAVIOUR_H

#include "AINode.h"
#include "Vector3.h"

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
	bool m_bplayerInSight;
	bool m_bstartShooting;
	float m_fdistanceToPlayer;
	bool m_bstartWalk01;
	bool m_bstartWalk02;
};

class CheckPlayerInRangeTask : public AITree::Node
{
private:
	AIBehaviour* status;
public:
	CheckPlayerInRangeTask(AIBehaviour* status) : status(status) {}
	virtual bool run() override
	{
		if (status->m_fdistanceToPlayer < 200)
		{
		//	std::cout << "Player in range" << std::endl;
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
		//	std::cout << "Approaching Player" << std::endl;
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

//class CheckIfGunHeldTask : public AITree::Node
//{
//private:
//	AIBehaviour* status;
//	bool gunHeld;
//public:
//	CheckIfGunHeldTask(AIBehaviour *status, bool _gunHeld) : status(status), gunHeld(false) {}
//	{
//		if (gunHeld)
//			return true;
//		else
//			return false;
//	}
//};

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
			//std::cout << "Shooting Player" << std::endl;
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

class IdleTask : public AITree::Node
{
private:
	AIBehaviour *status;
public:
	IdleTask(AIBehaviour* status) : status(status) {}
	virtual bool run() override
	{
		if (status->m_fdistanceToPlayer > 200)
		{
			//std::cout << "AI on idle" << std::endl;
			return true;
		}
		return false;
	}
};
#endif
