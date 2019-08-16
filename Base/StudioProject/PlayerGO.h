#ifndef PLAYER_GO_H
#define PLAYER_GO_H

#include "../Core/GameObject.h"
#include "GOList.h"
#include "FirstPersonCamera.h"

class PlayerGO : public GameObject
{
	enum CURRENT_POSTURE
	{
		STAND = 0,
		CROUCH,
		PRONE,
		NUM_POSTURE,
	};
	CURRENT_POSTURE theCurrentPosture;
public:
	GOList &gl = GOList::GetInstance();


	FirstPersonCamera camera;

	float m_fplayerSpeed;
	float m_fplayerHeight;
	bool m_bStopForward;

	PlayerGO();
	~PlayerGO();

	void Init();
	void Update(double dt);
};

#endif
