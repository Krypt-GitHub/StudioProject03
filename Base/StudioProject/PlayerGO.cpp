#include "PlayerGO.h"
#include "../Source/Application.h"


PlayerGO::PlayerGO()
	: m_bIsJumping(false)
	, m_bIsFalling(false)
	, m_bStopForward(false)
	, m_dElapsedTime(0.f)
	, m_fFallAcceleration(-10.f)
	, m_fFallSpeed(0.f)
	, m_fJumpSpeed(30.f)
	, m_fJumpAcceleration(-10.f)
	, m_fHealth(100.f)
	, m_fStrength(0.f)
	, gun(NULL)
{
	std::cout << "Player GameObject Created" << std::endl;
}


PlayerGO::~PlayerGO()
{
	std::cout << "Player GameObject Destroyed" << std::endl;
}

void PlayerGO::Init()
{
	theCurrentPosture = STAND;

	m_fplayerHeight = 40.f;
	camera.Init(Vector3(transform.position.x, transform.position.y + m_fplayerHeight, transform.position.z), Vector3(0, 0, 0), Vector3(0, 1, 0));
}

void PlayerGO::Update(double dt)
{
	static bool m_bRBDown = false;
	m_v3dir = (camera.target - camera.position).Normalized();

	if (Application::GetKeyDown(VK_SHIFT))
		m_fplayerSpeed = 40.f;
	else
		m_fplayerSpeed = 20.f;

	if (Application::GetKeyDown('W') || Application::GetKeyDown('A') || Application::GetKeyDown('S') || Application::GetKeyDown('D') || Application::GetKeyDown('Q'))
		Application::SetShouldUpdate(true);
	else
		Application::SetShouldUpdate(false);

	if (Application::GetKeyDown('W') && !m_bStopForward)
	{
		Vector3 view = (camera.target - camera.position).Normalized();
		Vector3 move = view * m_fplayerSpeed * dt;
		transform.position += Vector3(move.x, 0, move.z);
		camera.position += Vector3(move.x, 0, move.z);;
		camera.target += Vector3(move.x, 0, move.z);
	}

	if (Application::GetKeyDown('A'))
	{
		Vector3 view = (camera.target - camera.position).Normalized();
		Vector3 right = view.Cross(camera.up);
		right.y = 0;
		right.Normalize();
		transform.position += -right * m_fplayerSpeed * dt;
		camera.position += -right * m_fplayerSpeed * dt;
		camera.target += -right * m_fplayerSpeed * dt;
	}

	if (Application::GetKeyDown('S'))
	{
		Vector3 view = (camera.target - camera.position).Normalized();
		Vector3 move = -view * m_fplayerSpeed * dt;
		transform.position += Vector3(move.x, 0, move.z);
		camera.position += Vector3(move.x, 0, move.z);
		camera.target += Vector3(move.x, 0, move.z);
	}

	if (Application::GetKeyDown('D'))
	{
		Vector3 view = (camera.target - camera.position).Normalized();
		Vector3 right = view.Cross(camera.up);
		right.y = 0;
		right.Normalize();
		transform.position += right * m_fplayerSpeed * dt;
		camera.position += right * m_fplayerSpeed * dt;
		camera.target += right * m_fplayerSpeed * dt;
	}

	{
		Vector3 view = (camera.target - camera.position).Normalized();
		float yaw = (float)(-100.f * Application::camera_yaw * (float)dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		camera.target = camera.position + view;
		Vector3 right = view.Cross(camera.up);
		right.y = 0;
		right.Normalize();
		camera.up = right.Cross(view).Normalized();
	}
	{
		float pitch = (float)(-100.f * Application::camera_pitch * (float)dt);
		Vector3 view = (camera.target - camera.position).Normalized();
		Vector3 right = view.Cross(camera.up);
		right.y = 0;
		right.Normalize();
		camera.up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		camera.target = camera.position + view;
	}

	if (Application::GetKeyDown(VK_SPACE) && IsGrounded())
	{
		//soundEngine.PlayASound("Jump");
		SetToJump(true);
	}
	if (Application::GetMouseDown(1) && !m_bRBDown)
	{
		GameObject* pistol = gun;
		pistol->m_v3vel.SetZero();
		//Throwing physics
		gun = nullptr;

		m_bRBDown = true;
	}
	else if (!Application::GetMouseDown(1) && m_bRBDown)
	{
		m_bRBDown = false;
	}
	UpdateJump(dt);
	UpdateFall(dt);





}

void PlayerGO::UpdateJump(double dt)
{
	if (m_bIsJumping == false)
		return;

	m_dElapsedTime += dt;

	transform.position.y += m_fJumpSpeed * m_dElapsedTime + 0.5 * m_fJumpAcceleration * m_dElapsedTime * m_dElapsedTime;
	camera.position.y += m_fJumpSpeed * m_dElapsedTime + 0.5 * m_fJumpAcceleration * m_dElapsedTime * m_dElapsedTime;
	camera.target.y += m_fJumpSpeed * m_dElapsedTime + 0.5 * m_fJumpAcceleration * m_dElapsedTime * m_dElapsedTime;

	m_fJumpSpeed = m_fJumpSpeed + m_fFallAcceleration * m_dElapsedTime;

	if (m_fJumpSpeed < 0.f)
	{
		m_fJumpSpeed = 0.f;
		m_bIsJumping = false;
		m_fFallSpeed = 0.f;
		m_bIsFalling = true;
		m_dElapsedTime = 0.f;
	}
}

void PlayerGO::UpdateFall(double dt)
{
	if (m_bIsFalling == false)
		return;

	m_dElapsedTime += dt;

	transform.position.y += m_fFallSpeed * m_dElapsedTime + 0.5 * m_fFallAcceleration * m_dElapsedTime * m_dElapsedTime;
	camera.position.y += m_fFallSpeed * m_dElapsedTime + 0.5 * m_fFallAcceleration * m_dElapsedTime * m_dElapsedTime;
	camera.target.y += m_fFallSpeed * m_dElapsedTime + 0.5 * m_fFallAcceleration * m_dElapsedTime * m_dElapsedTime;

	m_fFallSpeed = m_fFallSpeed + m_fFallAcceleration * m_dElapsedTime;

	if (transform.position.y < 0)
	{
		transform.position.y = m_fplayerHeight;
		camera.position.y = transform.position.y + m_fplayerHeight;
		camera.target = camera.position + m_v3dir;
		m_fFallSpeed = 0.f;
		m_bIsFalling = false;
		m_dElapsedTime = 0.f;
	}
}

bool PlayerGO::IsGrounded()
{
	if (!m_bIsJumping && !m_bIsFalling)
		return true;
	else
		return false;
}

bool PlayerGO::IsJumping()
{
	if (m_bIsJumping && !m_bIsFalling)
		return true;
	else
		return false;
}

bool PlayerGO::IsFalling()
{
	if (!m_bIsJumping && m_bIsFalling)
		return true;
	else
		return false;
}

void PlayerGO::SetToFall(bool _isFalling)
{
	if (_isFalling)
	{
		m_bIsJumping = false;
		m_bIsFalling = true;
	}
}

void PlayerGO::SetToJump(bool _isJumping)
{
	if (_isJumping)
	{
		m_bIsJumping = true;
		m_bIsFalling = false;
		m_fJumpSpeed = 10.f;
	}
}

void PlayerGO::StopVertMove()
{
	m_bIsJumping = false;
	m_bIsFalling = false;
}