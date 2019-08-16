#include "PlayerGO.h"
#include "../Source/Application.h"


PlayerGO::PlayerGO()
{
}


PlayerGO::~PlayerGO()
{
}

void PlayerGO::Init()
{
	theCurrentPosture = STAND;

	m_fplayerHeight = 150.f;
	camera.Init(Vector3(transform.position.x, transform.position.y, transform.position.z), Vector3(0, 0, 0), Vector3(0, 1, 0));
}

void PlayerGO::Update(double dt)
{
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
		transform.position += -right * m_fplayerSpeed * dt;
		//player->SimpleMove(-right * m_fPlayerSpeed * dt);
		//controller.transform.position.y = terrainHeight + m_fPlayerHeight;
		camera.target += -right * m_fplayerSpeed * dt;
	}

	if (Application::GetKeyDown('S'))
	{
		Vector3 view = (camera.target - camera.position).Normalized();
		Vector3 move = -view * m_fplayerSpeed * dt;
		transform.position += Vector3(move.x, 0, move.z);
		//controller.transform.position.y = terrainHeight + m_fPlayerHeight;
		camera.target += Vector3(move.x, 0, move.z);
	}

	if (Application::GetKeyDown('D'))
	{
		Vector3 view = (camera.target - camera.position).Normalized();
		Vector3 right = view.Cross(camera.up);
		right.y = 0;
		transform.position += right * m_fplayerSpeed * dt;
		//controller.transform.position.y = terrainHeight + m_fPlayerHeight;
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

	camera.position.Set(transform.position.x, transform.position.y, transform.position.z);
}
