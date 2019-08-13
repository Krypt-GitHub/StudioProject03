#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "../Core/GameObject.h"
#include "../StudioProject/FirstPersonCamera.h"
#include "GOList.h"
//#include "../Source/SoundEngine.h"

class PlayerController
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

	GameObject *player;
	FirstPersonCamera camera;

	bool m_bIsJumping;
	float m_fJumpSpeed;
	bool m_bIsFalling;
	float m_fFallSpeed;
	float m_fFallAcceleration;
	float m_fJumpAcceleration;
	double m_dElapsedTime;

	float m_fPlayerSpeed;

	float m_fPlayerHeight;

	float m_fTerrainHeight;

	float m_fHealth;
	float m_fStrength;
	bool m_bStopForward;

	bool isZDown;

	PlayerController();
	~PlayerController();

	void Init();
	void Update(double dt);
	void UpdateJump(double dt);
	void UpdateFall(double dt);

	bool IsGrounded();
	bool IsJumping();
	bool IsFalling();
	void SetToFall(bool _isFalling);
	void SetToJump(bool _isJumping);
	void StopVertMove();
};

#endif
