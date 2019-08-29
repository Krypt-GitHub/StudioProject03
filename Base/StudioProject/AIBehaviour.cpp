#include "AIBehaviour.h"



AIBehaviour::AIBehaviour()
	: m_bdodge(false)
	, m_bstartWalk01(false)
	, m_bstartWalk02(true)
{
}


AIBehaviour::~AIBehaviour()
{
}

AIBehaviour::AIBehaviour(bool _playerInSight, float _distanceToPlayer)
	: m_bdodge(false)
	, m_bstartWalk01(false)
	, m_bstartWalk02(true)
{
	m_fdistanceToPlayer = _distanceToPlayer;
}
