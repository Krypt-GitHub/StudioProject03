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
};

class CheckPlayerInSightTask : public AITree::Node
{
private:
	AIBehaviour* status;
public:
	CheckPlayerInSightTask(AIBehaviour* status) : status(status) {}
	virtual bool run() override
	{
		if (status->m_fdistanceToPlayer < 100)
		{
			//std::cout << "Player in sight" << std::endl;
			return true;
		}
		return false;
	}
};

class ApproachPlayerTask : public AITree::Node
{
private:
	AIBehaviour* status;
	bool obstructed;
public:
	ApproachPlayerTask(AIBehaviour *status, bool obstructed) : status(status), obstructed(obstructed) {}
	virtual bool run() override
	{
		if (obstructed)
			return false;
		if (status->m_fdistanceToPlayer < 50)
		{
			status->m_bplayerInSight = true;
			//std::cout << "Approaching Player" << std::endl;

		}
		return true;
	}
};

//class ShootPlayerTask : public AITree::Node
//{
//private:
//	AIBehaviour* status;
//public:
//	ShootPlayerTask(AIBehaviour *status)
//};
#endif
