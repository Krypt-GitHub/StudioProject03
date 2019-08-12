#include "PlayerController.h"
#include "../Source/Application.h"


PlayerController::PlayerController()
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
{
}

PlayerController::~PlayerController()
{
	if (player != NULL)
	{
		delete player;
		player = NULL;
	}
}

void PlayerController::Init()
{
	player = new GameObject;
	player->SetActive(true);
	player->isStatic = false;
	player->transform.position.Set(10, 30, 10);

	isZDown = false;

	theCurrentPosture = STAND;

	m_fPlayerHeight = 100.f;
	camera.Init(Vector3(player->transform.position.x, player->transform.position.y, player->transform.position.z), Vector3(0, 0, 0), Vector3(0, 1, 0));
}

void PlayerController::Update(double dt)
{
	//m_fTerrainHeight = terrainHeight;
	//std::cout << m_fTerrainHeight << std::endl;
	if (Application::GetKeyDown(VK_SHIFT))
		m_fPlayerSpeed = 40.f;
	else
		m_fPlayerSpeed = 20.f;

	if (Application::GetKeyDown('W') && !m_bStopForward)
	{
		Vector3 view = (camera.target - camera.position).Normalized();
		Vector3 move = view * m_fPlayerSpeed * dt;
		player->transform.position += Vector3(move.x, 0, move.z);
		//player->SimpleMove(Vector3(move.x, 0, move.z));
		//controller.transform.position.y = terrainHeight + m_fPlayerHeight;
		//camera.target.y += move.y;//controller.transform.position.y + (camera.target.y - controller.transform.position.y);
		camera.target += Vector3(move.x, 0, move.z);
	}

	if (Application::GetKeyDown('A'))
	{
		Vector3 view = (camera.target - camera.position).Normalized();
		Vector3 right = view.Cross(camera.up);
		right.y = 0;
		right.Normalize();
		player->transform.position += -right * m_fPlayerSpeed * dt;
		//player->SimpleMove(-right * m_fPlayerSpeed * dt);
		//controller.transform.position.y = terrainHeight + m_fPlayerHeight;
		camera.target += -right * m_fPlayerSpeed * dt;
	}

	if (Application::GetKeyDown('S'))
	{
		Vector3 view = (camera.target - camera.position).Normalized();
		Vector3 move = -view * m_fPlayerSpeed * dt;
		player->transform.position += Vector3(move.x, 0, move.z);
		//controller.transform.position.y = terrainHeight + m_fPlayerHeight;
		camera.target += Vector3(move.x, 0, move.z);
	}

	if (Application::GetKeyDown('D'))
	{
		Vector3 view = (camera.target - camera.position).Normalized();
		Vector3 right = view.Cross(camera.up);
		right.y = 0;
		right.Normalize();
		player->transform.position += right * m_fPlayerSpeed * dt;
		//controller.transform.position.y = terrainHeight + m_fPlayerHeight;
		camera.target += right * m_fPlayerSpeed * dt;
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


	if (!isZDown && Application::GetKeyDown('Z'))
	{
		isZDown = true;
		theCurrentPosture = (CURRENT_POSTURE)(theCurrentPosture + 1);
		if (theCurrentPosture == NUM_POSTURE)
			theCurrentPosture = STAND;

		Vector3 viewDirection = camera.target - camera.position;

		switch(theCurrentPosture)
		{
		case STAND:
			player->transform.position.y = m_fTerrainHeight + 100.f;
			camera.target = camera.position + viewDirection;
			m_fPlayerSpeed = 20;
			m_fPlayerHeight = 100.f;
			break;
		case CROUCH:
			player->transform.position.y = m_fTerrainHeight + 50.f;
			camera.target = camera.position + viewDirection;
			m_fPlayerSpeed = 15;
			m_fPlayerHeight = 50.f;
			break;
		case PRONE:			
			player->transform.position.y = m_fTerrainHeight + 20.f;
			camera.target = camera.position + viewDirection;
			m_fPlayerSpeed = 5;
			m_fPlayerHeight = 20.f;
			break;
		default:
			break;
		}
	}
	if (isZDown && !Application::GetKeyDown('Z'))
	{
		isZDown = false;
	}

	UpdateJump(dt);
	UpdateFall(dt);

	camera.position.Set(player->transform.position.x, player->transform.position.y, player->transform.position.z);
}

void PlayerController::UpdateJump(double dt)
{
	if (m_bIsJumping == false)
		return;

	m_dElapsedTime += dt;

	player->transform.position.y += m_fJumpSpeed * m_dElapsedTime + 0.5 * m_fJumpAcceleration * m_dElapsedTime * m_dElapsedTime;
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

void PlayerController::UpdateFall(double dt)
{
	if (m_bIsFalling == false)
		return;

	m_dElapsedTime += dt;

	player->transform.position.y += m_fFallSpeed * m_dElapsedTime + 0.5 * m_fFallAcceleration * m_dElapsedTime * m_dElapsedTime;
	camera.target.y += m_fFallSpeed * m_dElapsedTime + 0.5 * m_fFallAcceleration * m_dElapsedTime * m_dElapsedTime;

	m_fFallSpeed = m_fFallSpeed + m_fFallAcceleration * m_dElapsedTime;

	if (player->transform.position.y < 0)
	{
		Vector3 viewDir = camera.target - player->transform.position;
		player->transform.position.y = m_fPlayerHeight;
		camera.target = player->transform.position + viewDir;
		m_fFallSpeed = 0.f;
		m_bIsFalling = false;
		m_dElapsedTime = 0.f;
	}
}

bool PlayerController::IsGrounded()
{
	if (!m_bIsJumping && !m_bIsFalling)
		return true;
	else
		return false;
}

bool PlayerController::IsJumping()
{
	if (m_bIsJumping && !m_bIsFalling)
		return true;
	else
		return false;
}

bool PlayerController::IsFalling()
{
	if (!m_bIsJumping && m_bIsFalling)
		return true;
	else
		return false;
}

void PlayerController::SetToFall(bool _isFalling)
{
	if (_isFalling)
	{
		m_bIsJumping = false;
		m_bIsFalling = true;
	}
}

void PlayerController::SetToJump(bool _isJumping)
{
	if (_isJumping)
	{
		m_bIsJumping = true;
		m_bIsFalling = false;
		m_fJumpSpeed = 10.f;
	}
}

void PlayerController::StopVertMove()
{
	m_bIsJumping = false;
	m_bIsFalling = false;
}
