#ifndef PLAYER_GO_H
#define PLAYER_GO_H

#include "../Core/GameObject.h"
#include "GOList.h"
#include "FirstPersonCamera.h"
#include "PistolGO.h"

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
	
	PistolGO* gun;


	bool m_bIsJumping;
	float m_fJumpSpeed;
	bool m_bIsFalling;
	float m_fFallSpeed;
	float m_fFallAcceleration;
	float m_fJumpAcceleration;
	double m_dElapsedTime;
	float m_fplayerSpeed;

	float m_fplayerHeight;

	float m_fHealth;
	float m_fStrength;
	bool m_bStopForward;

	bool isZDown;

	PlayerGO();
	virtual ~PlayerGO();

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
