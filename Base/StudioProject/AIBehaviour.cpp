#include "AIBehaviour.h"



AIBehaviour::AIBehaviour()
{
}


AIBehaviour::~AIBehaviour()
{
}

AIBehaviour::AIBehaviour(bool _playerInSight, float _distanceToPlayer)
{
	m_bplayerInSight = _playerInSight;
	m_fdistanceToPlayer = _distanceToPlayer;
}
