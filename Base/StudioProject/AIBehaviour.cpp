#include "AIBehaviour.h"



AIBehaviour::AIBehaviour()
	: m_bdodge(false)
{
}


AIBehaviour::~AIBehaviour()
{
}

AIBehaviour::AIBehaviour(bool _playerInSight, float _distanceToPlayer)
	: m_bdodge(false)
{
	//m_bplayerInSight = _playerInSight;
	m_fdistanceToPlayer = _distanceToPlayer;
}
